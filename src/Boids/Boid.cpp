#include "Boid.h"
#include <variant>
#include "Boids/Iterator/ForEachBoidMovement.h"

namespace Boids {

Boid::Boid(unsigned int speciesId, ShapesType const& shape, utils::TransformAttributes const& transformAttributes, BehaviorConfig const& behaviorConfig, ForcesConfig const& forcesConfig)
    : _speciesId(speciesId), _shape(shape), _movement(speciesId, transformAttributes, behaviorConfig, forcesConfig)
{}

void Boid::update(std::vector<Boid> const& boids, Features::ObstaclesManager const& obstacles, Features::FoodProvider& foodProvider)
{
    auto const boidsIterator = Iterator::ForEachBoidMovement<Boid>{boids};
    _movement.update(boidsIterator, obstacles, foodProvider, getRadius());
}

void Boid::draw(bool depthMapDraw, glm::mat4 lightSpaceMatrix)
{
    std::visit(
        [&](auto const& shape) {
            if (depthMapDraw)
                shape.drawDepthMap(_movement.getTransformAttributes(), lightSpaceMatrix);
            else
                shape.draw(_movement.getTransformAttributes(), lightSpaceMatrix);
        },
        _shape
    );
}

float Boid::getRadius() const
{
    float radius = 0.f;
    std::visit([&](auto const& shape) { radius = shape.getRadius(); }, _shape);
    return radius;
}

} // namespace Boids