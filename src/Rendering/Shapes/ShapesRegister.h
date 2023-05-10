#pragma once

#include "Rendering/Shapes/List/Cone.h"
#include "Rendering/Shapes/List/Duck.h"
#include "Rendering/Shapes/List/Sphere.h"

using BoidsShapesType = std::variant<Rendering::Shapes::Sphere, Rendering::Shapes::Cone, Rendering::Shapes::Duck>;

namespace Rendering::Shapes {

BoidsShapesType const& getSphereInstance(float radius);
BoidsShapesType const& getConeInstance(float radius);
BoidsShapesType const& getDuckInstance();

} // namespace Rendering::Shapes
