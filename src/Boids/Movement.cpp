#include "Movement.h"
#include "Boids/Calculator/forcesCalculator.h"
#include "utils/vec.h"

namespace Boids {

Movement::Movement(unsigned int speciesId, utils::TransformAttributes const& transformAttributes, BehaviorConfig& behaviorConfig, ForcesConfig& forcesConfig)
    : _speciesId(speciesId), _transformAttributes(transformAttributes), _behaviorConfig(behaviorConfig), _forcesConfig(forcesConfig)
{}

void Movement::update(Iterator::IForEachBoidMovement const& boids, Features::ObstaclesManager const& obstacles, Features::FoodProvider& foodProvider, float sceneRadius, float boidRadius)
{
    // Add forces to acceleration
    addFoodAttraction(foodProvider);
    addClassicBoidsForces(boids, boidRadius);
    addWallSteering(sceneRadius);
    addObstaclesAvoidance(obstacles, boidRadius);

    addToVelocity(getAcceleration());
    utils::vec::constrain(_transformAttributes._velocity, _behaviorConfig._minSpeed, _behaviorConfig._maxSpeed);

    addToPosition(getVelocity());
    resetForces();
}

void Movement::addFoodAttraction(Features::FoodProvider& foodProvider)
{
    addToAcceleration(Boids::Calculator::computeFoodAttraction(*this, foodProvider, _behaviorConfig._foodAttractionRadius));
}

void Movement::addObstaclesAvoidance(Features::ObstaclesManager const& obstacles, float boidRadius)
{
    addToAcceleration(Boids::Calculator::computeObstaclesAvoidance(*this, obstacles, boidRadius));
}

void Movement::addClassicBoidsForces(Iterator::IForEachBoidMovement const& boids, float boidRadius)
{
    auto const separation = Boids::Calculator::computeSeparationForce(*this, getNearbyBoids(boids, boidRadius, _forcesConfig._separationRadius)) * _forcesConfig._separationFactor;
    auto const alignment  = Boids::Calculator::computeAlignmentForce(*this, getNearbyAndSameBoids(boids, boidRadius, _forcesConfig._alignmentRadius)) * _forcesConfig._alignmentFactor;
    auto const cohesion   = Boids::Calculator::computeCohesionForce(*this, getNearbyAndSameBoids(boids, boidRadius, _forcesConfig._cohesionRadius)) * _forcesConfig._cohesionFactor;

    addToAcceleration(separation);
    addToAcceleration(alignment);
    addToAcceleration(cohesion);
}

void Movement::addWallSteering(float sceneRadius)
{
    auto applyWallSteering = [&](float positionOnAxe, glm::vec3 turnFactor) {
        if (positionOnAxe <= -sceneRadius)
            addToAcceleration(turnFactor);
        else if (positionOnAxe >= sceneRadius)
            addToAcceleration(-turnFactor);
    };

    applyWallSteering(_transformAttributes._position.x, glm::vec3(_forcesConfig._turnFactor, 0.f, 0.f));
    applyWallSteering(_transformAttributes._position.y, glm::vec3(0.f, _forcesConfig._turnFactor, 0.f));
    applyWallSteering(_transformAttributes._position.z, glm::vec3(0.f, 0.f, _forcesConfig._turnFactor));
}

static std::vector<Movement> getNearbyBoidsFromBoid(
    std::optional<unsigned int> const&    speciesId,
    Movement const&                       scannedBoid,
    Iterator::IForEachBoidMovement const& boidsIterator,
    float                                 boidRadius,
    float                                 proximityRadius
)
{
    std::vector<Movement> nearbyBoids{};
    boidsIterator.loop([&](Movement const& boid) {
        if (boid.getPosition() == scannedBoid.getPosition())
            return;

        /// If no species is specified, we add every close boids.
        /// If a species is specified, we add close boids having this species.
        float const actualDistance = glm::distance(scannedBoid.getPosition(), boid.getPosition()) - boidRadius - boidRadius;
        bool const  hasSameSpecies = speciesId.has_value() && boid.getSpeciesId() == speciesId;
        if (actualDistance < proximityRadius && (!speciesId.has_value() || hasSameSpecies))
            nearbyBoids.push_back(boid);
    });
    return nearbyBoids;
}

std::vector<Movement> Movement::getNearbyBoids(Iterator::IForEachBoidMovement const& boids, float boidRadius, float proximityRadius) const
{
    return getNearbyBoidsFromBoid(std::nullopt, *this, boids, boidRadius, proximityRadius);
}

std::vector<Movement> Movement::getNearbyAndSameBoids(Iterator::IForEachBoidMovement const& boids, float boidRadius, float proximityRadius) const
{
    return getNearbyBoidsFromBoid(getSpeciesId(), *this, boids, boidRadius, proximityRadius);
}

} // namespace Boids