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
        auto* window = ctx.underlying_glfw_window();
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        {
            rotateLeft(move.delta.x * 100);
            rotateUp(move.delta.y * 100);
        }
    };
}

void FirstPerson::computeDirectionVectors()
{
    _marker._front = glm::vec3{cos(_verticalRotation) * sin(_horizontalRotation), sin(_verticalRotation), cos(_verticalRotation) * cos(_horizontalRotation)};
    _marker._left  = glm::vec3{sin(_horizontalRotation + p6::PI / 2), 0, cos(_horizontalRotation + p6::PI / 2)};
    _marker._up    = glm::cross(_marker._front, _marker._left);
}

} // namespace Camera