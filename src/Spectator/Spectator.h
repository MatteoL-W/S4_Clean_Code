#pragma once

#include "Controls.h"
#include "Rendering/Shapes/ShapesRegister.h"

namespace Spectator {

class Spectator {
public:
    Spectator();
    void draw(utils::RenderingDatas& renderingDatas) const;

private:
    Controls                   _controls;
    SpectatorShapesType const& _shape{Rendering::Shapes::getSpectatorShapeInstance()};
};

} // namespace Spectator
