#pragma once

#include <p6/p6.h>
#include <iterator>
#include <list>
#include "utils/RenderingDatas.h"
#include "utils/StrongType.h"

namespace Features {
using Clock = std::chrono::high_resolution_clock;

struct FoodConfig {
    /// The food providing interval in seconds.
    int _providingInterval = 10;

    /// The number of food dropping every interval
    int _drops = 2;

    /// The radius of the food
    float _radius = 0.3f;
};

class FoodProvider {
public:
    explicit FoodProvider(FoodConfig const& foodConfig, bool enableDropsInstantly = true);

    /// Enable the food dropping randomly in the map
    void enableDrop();
    void update(SceneRadius&);
    void draw(utils::RenderingDatas& renderingDatas) const;
    void erase(std::list<glm::vec3>::const_iterator it) { _foods.erase(it); };
    void reset() { _foods.clear(); };

    std::list<glm::vec3> const& getFood() const { return _foods; };
    float                       getFoodRadius() const { return _config._radius; };
    FoodConfig&                 getConfig() { return _config; };

private:
    /// Add a new food randomly inside the map
    void addFoodRandomly(SceneRadius&);

private:
    std::optional<Clock::time_point> _randomFoodStartTime;
    std::list<glm::vec3>             _foods{};
    FoodConfig                       _config{};
};

} // namespace Features