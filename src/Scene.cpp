#include "Scene.h"
#include "GUI/GUI.hpp"
#include "Rendering/Cameras/CameraManager.h"

void Scene::setupWorld(p6::Context& ctx)
{
    initializeBoids(_sceneRadius);
    initializeLights();
    initializeSkyBox();
    initializeObstacles();
    initializeImGui(ctx.imgui);

    _cameraManager.handleEvents(ctx);

    ctx.update = [&]() {
        updateMembers(ctx);
        renderDepthMap();
        render({ctx.main_canvas_width(), ctx.main_canvas_height()});
    };
}
void Scene::initializeObstacles()
{
    _obstaclesManager.addOne({0.f, -_sceneRadius.value + 0.5f, 0.f}, 1.f, true);
}

void Scene::initializeBoids(SceneRadius& sceneRadius)
{
    _firstSpecies._quantity       = 10;
    _firstSpecies._behaviorConfig = {._minSpeed = .050f, ._maxSpeed = 0.075f, ._foodAttractionRadius = 1.4f};

    _secondSpecies._quantity       = 3;
    _secondSpecies._behaviorConfig = {._minSpeed = .020f, ._maxSpeed = 0.1f, ._foodAttractionRadius = 4.5f};
    _secondSpecies._forcesConfig   = {._separationRadius = 0.1f, ._alignmentFactor = 0.f, ._cohesionFactor = 0.f};

    _thirdSpecies._quantity       = 5;
    _thirdSpecies._behaviorConfig = {._minSpeed = .060f, ._maxSpeed = 0.085f};
    _thirdSpecies._forcesConfig   = {._cohesionRadius = 3.f, ._cohesionFactor = 0.2f};

    _demoLODSpecies._quantity = 0;

    loadSpecies(sceneRadius);
}

void Scene::loadSpecies(SceneRadius& sceneRadius)
{
    _boidsManager.addSpecies(sceneRadius, _firstSpecies);
    _boidsManager.addSpecies(sceneRadius, _secondSpecies);
    _boidsManager.addSpecies(sceneRadius, _thirdSpecies);
    _boidsManager.addSpecies(sceneRadius, _demoLODSpecies);
}

void Scene::initializeLights()
{
    _renderingDatas._directional = Rendering::Lights::Directional{{10.f, 7.f, -5.f}, {0.f, 0.f, 0.f}, Rendering::Lights::Intensity{.07f, .2f, .3f}, {1.f, 1.f, 1.f}};
    _renderingDatas._points      = {
        Rendering::Lights::Point{Spectator::getSpectatorPosition(), Rendering::Lights::Intensity{.01f, .3f, .4f}, {1.f, 1.f, 1.f}, 1.f, .09f, .032f},
    };
}

void Scene::initializeSkyBox()
{
    _skyBox.setFaces(
        {"assets/skybox/new/px.png",
         "assets/skybox/new/nx.png",
         "assets/skybox/new/ny.png",
         "assets/skybox/new/py.png",
         "assets/skybox/new/pz.png",
         "assets/skybox/new/nz.png"}
    );
}

void Scene::initializeImGui(std::function<void()>& imguiFn)
{
    imguiFn = [this]() {
        ImGui::Begin("GOOD BOIDS");

        ImGui::SeparatorText("Species");
        if (ImGui::BeginTabBar("Species"))
        {
            GUI::showSpeciesGUI("Majority", _firstSpecies);
            GUI::showSpeciesGUI("Eaters", _secondSpecies);
            GUI::showSpeciesGUI("Gangsters", _thirdSpecies);
            GUI::showSpeciesGUI("LOD Demo", _demoLODSpecies);
            ImGui::EndTabBar();
        }
        if (ImGui::Button("Reload all boids"))
        {
            _boidsManager.reset();
            loadSpecies(_sceneRadius);
        }

        if (ImGui::Button("Reset parameters and reload all boids"))
        {
            _boidsManager.reset();
            initializeBoids(_sceneRadius);
        }

        GUI::showCameraGUI();

        GUI::showDirectionalLightGUI(_renderingDatas._directional);
        GUI::showPointLightsGUI(_renderingDatas._points);
        if (ImGui::Button("Reload lights")) {
            _renderingDatas._directional.reset();
            _renderingDatas._points.clear();
            initializeLights();
        }

        GUI::showObstacleGUI(_obstaclesManager.getConfig());
        if (ImGui::Button("Reset obstacles"))
            _obstaclesManager.reset();
        if (ImGui::Button("Add house obstacle"))
            initializeObstacles();

        GUI::showFoodGUI(_foodProvider.getConfig());
        if (ImGui::Button("Reset foods"))
            _foodProvider.reset();

        ImGui::End();
    };
}

void Scene::updateMembers(p6::Context& ctx)
{
    _renderingDatas._lightSpaceMatrix = _shadowMap.getLightSpaceMatrix();
    _renderingDatas._points[0].setPosition(Spectator::getSpectatorPosition());

    _cameraManager.updateEvents(ctx, _sceneRadius);
    _foodProvider.update(_sceneRadius);
    _boidsManager.update(_obstaclesManager, _foodProvider, _sceneRadius);
    _obstaclesManager.update(_sceneRadius);
}

void Scene::renderDepthMap()
{
    if (!_renderingDatas._directional.has_value())
        return;

    _shadowMap.renderDepthMap(
        [&](glm::mat4 lightSpaceMatrix) {
            auto renderingDatas = utils::RenderingDatas{
                ._renderType       = utils::RenderType::DepthMap,
                ._lightSpaceMatrix = lightSpaceMatrix,
            };
            _boidsManager.draw(renderingDatas);
            _spectator.draw(renderingDatas);
            _fence.draw(utils::TransformAttributes{._position = {0.f, -_sceneRadius.value, 0.f}}, renderingDatas);
            _house.draw({._position = {0.f, -_sceneRadius.value, 0.f}}, renderingDatas);
        },
        _renderingDatas._directional.value()
    );
}

void Scene::render(glm::ivec2 canvasDimensions)
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glViewport(0, 0, canvasDimensions.x, canvasDimensions.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shadowMap.bindTextureUnit(1);

    _floor.draw({._position = glm::vec3{0.f, -_sceneRadius.value + .1f, 0.f}}, _renderingDatas);
    _boidsManager.draw(_renderingDatas);
    _spectator.draw(_renderingDatas);
    _obstaclesManager.draw(_renderingDatas);
    _foodProvider.draw(_renderingDatas);
    _fence.draw(utils::TransformAttributes{._position = {0.f, -_sceneRadius.value, 0.f}}, _renderingDatas);
    _house.draw({._position = {0.f, -_sceneRadius.value, 0.f}}, _renderingDatas);

    _skyBox.draw();
    _cubeMap.draw();
}
