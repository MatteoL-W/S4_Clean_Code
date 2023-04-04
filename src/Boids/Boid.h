#pragma once

#include <p6/p6.h>
#include "BoidMovement.h"
#include "Boids/Configs.h"
#include "Food/FoodProvider.h"
#include "Obstacles/ObstaclesManager.h"
#include "Shapes/2D.h"
#include "utils/TransformAttributes.h"

class Boid {
public:
    explicit Boid(ShapesType const&, unsigned int _speciesId, Utils::TransformAttributes const&, BehaviorConfig const&, ForcesConfig const&);

    void update(std::vector<Boid> const&, ObstaclesManager const&, FoodProvider&);
    void draw(p6::Context&);

    void         setForcesConfig(ForcesConfig config) { _movement.setForcesConfig(config); };
    void         setBehaviorConfig(BehaviorConfig config) { _movement.setBehaviorConfig(config); };
    BoidMovement getMovement() const { return _movement; };

private:
    [[nodiscard]] float getRadius() const;

private:
    ShapesType   _shape{};
    BoidMovement _movement{};
};
