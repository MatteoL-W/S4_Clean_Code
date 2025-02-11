#include "FoodProvider.h"
#include <chrono>
#include "Rendering/Shapes/ShapesRegister.h"

namespace Features {

FoodProvider::FoodProvider(FoodConfig const& foodConfig, bool enableDropsInstantly)
    : _config(foodConfig)
{
    if (enableDropsInstantly)
        enableDrop();
};

void FoodProvider::enableDrop()
{
    _randomFoodStartTime = Clock::now();
}

void FoodProvider::update(SceneRadius& sceneRadius)
{
    if (!_randomFoodStartTime.has_value())
        return;

    auto current_time = Clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - *_randomFoodStartTime).count();
    if (elapsed_time >= _config._providingInterval)
    {
        addFoodRandomly(sceneRadius);
        _randomFoodStartTime = current_time;
    }
}

void FoodProvider::draw(utils::RenderingDatas& renderingDatas) const
{
    auto const& shape = Rendering::Shapes::getFoodShapeInstance(_config._radius);

    if (_foods.empty())
        return;

    for (auto const& food : _foods)
    {
        std::visit(
            [&](auto const& sphereShape) {
                sphereShape.draw(utils::TransformAttributes{food}, renderingDatas);
            },
            shape
        );
    }
}

void FoodProvider::addFoodRandomly(SceneRadius& sceneRadius)
{
    for (int i = 0; i < _config._drops; i++)
        _foods.emplace_back(
            p6::random::number(-sceneRadius.value, sceneRadius.value),
            p6::random::number(-sceneRadius.value + 2.5f, sceneRadius.value),
            p6::random::number(-sceneRadius.value, sceneRadius.value)
        );
}

} // namespace Features
