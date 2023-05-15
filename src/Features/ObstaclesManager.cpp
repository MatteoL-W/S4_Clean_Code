#include "ObstaclesManager.h"
#include "Rendering/Shapes/ShapesRegister.h"

namespace Features {

ObstaclesManager::ObstaclesManager(const ObstaclesConfig& config, bool enableDropsInstantly)
    : _config(config)
{
    if (enableDropsInstantly)
        enableDrop();
}

void ObstaclesManager::update(SceneRadius& sceneRadius)
{
    if (!_obstacles.empty())
    {
        // Erase obstacles that meet the condition
        _obstacles.erase(
            std::remove_if(_obstacles.begin(), _obstacles.end(), [&](SphereObstacle const& obstacle) { return obstacle._position.y < -sceneRadius.value; }),
            _obstacles.end()
        );

        // Modify remaining obstacles
        for (auto& obstacle : _obstacles)
            obstacle._position.y -= _config._fallingFactor;
    }

    if (!_randomDropStartTime.has_value())
        return;

    auto current_time = Clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - *_randomDropStartTime).count();
    if (elapsed_time >= _config._providing_interval)
    {
        addObstaclesRandomly(sceneRadius);
        _randomDropStartTime = current_time;
    }
}

void ObstaclesManager::addObstaclesRandomly(SceneRadius& sceneRadius)
{
    for (int i = 0; i < _config._drops; i++)
    {
        auto randomPosition = glm::vec3{
            p6::random::number(-sceneRadius.value, sceneRadius.value),
            sceneRadius.value,
            p6::random::number(-sceneRadius.value, sceneRadius.value)};
        addOne(randomPosition, _config._radius);
    }
}

void ObstaclesManager::draw()
{
    auto renderingDatas = utils::RenderingDatas{._renderType = utils::RenderType::Classic};
    for (auto const& obstacle : _obstacles)
    {
        if (!obstacle._visible)
            return;

        ObstaclesShapesType const& shape = Rendering::Shapes::getObstacleShapeInstance();
        std::visit(
            [&](auto const& sphereShape) {
                sphereShape.draw(utils::TransformAttributes{obstacle._position}, renderingDatas);
            },
            shape
        );
    }
}

void ObstaclesManager::addOne(glm::vec3 pos, float radius)
{
    _obstacles.push_back(SphereObstacle{glm::vec3{pos}, radius});
}

} // namespace Features