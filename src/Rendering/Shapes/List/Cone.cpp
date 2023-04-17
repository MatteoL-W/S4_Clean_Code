#include "Cone.h"
#include "Rendering/Cameras/CameraManager.h"
#include "Rendering/Engine/Mesh.h"

namespace Rendering::Shapes {

Cone::Cone(float radius)
    : _radius(radius), _vertices(Rendering::Geometries::cone_vertices(getRadius(), getRadius(), 32, 16)), _mesh(RenderEngine::Mesh{_vertices})
{}

void Cone::draw(p6::Context& ctx, utils::TransformAttributes const& transformAttributes) const
{
    _shader._program.use();
    auto cameraManager = Camera::getCameraInstance();

    auto const  up            = glm::vec3(0.f, 1.f, 0.f);
    auto const  rotationAxis  = glm::cross(up, transformAttributes._velocity);
    float const rotationAngle = glm::acos(glm::dot(up, transformAttributes._velocity));

    auto modelViewMatrix = cameraManager.getViewMatrix();
    modelViewMatrix      = glm::translate(modelViewMatrix, transformAttributes._position);
    modelViewMatrix      = glm::rotate(modelViewMatrix, rotationAngle, rotationAxis);

    auto projectionMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), .1f, 100.f);
    auto normalMatrix     = glm::transpose(glm::inverse(modelViewMatrix));

    glUniformMatrix4fv(_shader.uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
    glUniformMatrix4fv(_shader.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * modelViewMatrix));
    glUniformMatrix4fv(_shader.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    glBindVertexArray(_mesh.getVao());
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(_vertices.size()));
    glBindVertexArray(0);
}

} // namespace Rendering::Shapes