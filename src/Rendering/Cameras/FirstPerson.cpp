#include "FirstPerson.h"

namespace Camera {

FirstPerson::FirstPerson()
{
    computeDirectionVectors();
}

void FirstPerson::rotateLeft(float degrees)
{
    _horizontalRotation -= degrees * p6::PI / 180;
    computeDirectionVectors();
}

void FirstPerson::rotateUp(float degrees)
{
    _verticalRotation += degrees * p6::PI / 180;
    computeDirectionVectors();
}

void FirstPerson::handleEvents(p6::Context& ctx)
{
    ctx.mouse_moved = [&](p6::MouseMove move) {
        rotateLeft(move.delta.x * 100);
        rotateUp(move.delta.y * 100);
    };

    // ToDo: Is it necessary here ?
    Spectator::getControlsInstance().handleEvents(ctx, _front, _left);
}

void FirstPerson::computeDirectionVectors()
{
    _front = glm::vec3{cos(_verticalRotation) * sin(_horizontalRotation), sin(_verticalRotation), cos(_verticalRotation) * cos(_horizontalRotation)};
    _left  = glm::vec3{sin(_horizontalRotation + p6::PI / 2), 0, cos(_horizontalRotation + p6::PI / 2)};
    _up    = glm::cross(_front, _left);
}

} // namespace Camera