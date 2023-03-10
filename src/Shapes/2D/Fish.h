#pragma once

#include <p6/p6.h>
#include "utils/TransformAttributes.h"

namespace Shapes::TwoDimensions {

class Fish {
public:
    Fish() = default;

    explicit Fish(float radius)
        : _radius(radius){};

    void draw(p6::Context& ctx, TransformAttributes const& transformAttributes) const
    {
        ctx.equilateral_triangle(
            // Position in the inverse direction multiplied to be approximately 80% of the circle
            p6::Center{transformAttributes._position - 2.f * glm::vec2(_radius * 80) * (transformAttributes._velocity)},
            p6::Radius{static_cast<float>(_radius)},
            p6::Rotation{p6::Angle{transformAttributes._velocity}}
        );
        ctx.circle(
            p6::Center{transformAttributes._position},
            p6::Radius{_radius}
        );
    }

    void  setRadius(float radius) { _radius = radius; };
    float getRadius() const { return _radius; };

private:
    float _radius;
};

} // namespace Shapes::TwoDimensions
