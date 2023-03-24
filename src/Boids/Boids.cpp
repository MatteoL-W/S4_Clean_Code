#include "Boids.h"

void Boids::addSpecies(p6::Context& ctx, Species& species)
{
    for (int i = 0; i < species._quantity; i++)
        _boids.emplace_back(
            std::to_string(_speciesNumber) + "species",
            utils::TransformAttributes{
                ._position = glm::vec2{p6::random::number(-ctx.aspect_ratio(), ctx.aspect_ratio()), p6::random::number(-1, 1)},
                ._velocity = glm::vec2{p6::random::number(-0.001f, 0.001f), p6::random::number(-0.001f, 0.001f)},
            },
            species._shape,
            species._behaviorConfig,
            species._forcesConfig
        );

    _speciesNumber++;
}

void Boids::updateAndDraw(p6::Context& ctx, ObstaclesManager const& obstacles, FoodProvider& foodProvider)
{
    for (auto& boid : _boids)
    {
        boid.update(_boids, obstacles, foodProvider);
        boid.draw(ctx);
    }
}

void Boids::updateForcesConfig(ForcesConfig const& config)
{
    for (auto& boid : _boids)
        boid.setForcesConfig(config);
}

void Boids::updateBehaviorConfig(BehaviorConfig const& config)
{
    for (auto& boid : _boids)
        boid.setBehaviorConfig(config);
}
