#pragma once

#include "Rendering/Shapes/List/BlueCloud.h"
#include "Rendering/Shapes/List/Cloud.h"
#include "Rendering/Shapes/List/DebugLOD.h"
#include "Rendering/Shapes/List/Drop.h"
#include "Rendering/Shapes/List/PinkCloud.h"
#include "Rendering/Shapes/List/Sheep.h"
#include "Rendering/Shapes/List/Star.h"

using BoidsShapesType     = std::variant<Rendering::Shapes::Cloud, Rendering::Shapes::PinkCloud, Rendering::Shapes::BlueCloud, Rendering::Shapes::DebugLOD>;
using SpectatorShapesType = std::variant<Rendering::Shapes::Sheep>;
using ObstaclesShapesType = std::variant<Rendering::Shapes::Drop>;
using FoodShapesType      = std::variant<Rendering::Shapes::Star>;

namespace Rendering::Shapes {

BoidsShapesType const&     getCloudInstance();
BoidsShapesType const&     getPinkCloudInstance();
BoidsShapesType const&     getBlueCloudInstance();
BoidsShapesType const&     getDebugLODInstance();

SpectatorShapesType const& getSpectatorShapeInstance();
ObstaclesShapesType const& getDropShapeInstance();
FoodShapesType const&      getFoodShapeInstance(float radius);

} // namespace Rendering::Shapes
