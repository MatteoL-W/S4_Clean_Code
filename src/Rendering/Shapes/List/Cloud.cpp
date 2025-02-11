#include "Cloud.h"
#include <glm/gtx/quaternion.hpp>
#include "Rendering/Engine/Mesh.h"

namespace Rendering::Shapes {

Cloud::Cloud()
    : _LODHandler(
        {Engine::Model{"assets/models/Cloud/cute_cloud.gltf", false},
         Engine::Model{"assets/models/Cloud/cute_cloud_mid.gltf", false},
         Engine::Model{"assets/models/Cloud/cute_cloud_low.gltf", false}},
        10.f
    )
{}

void Cloud::draw(utils::TransformAttributes const& transformAttributes, utils::RenderingDatas& renderingDatas) const
{
    glCullFace(GL_FRONT);

    auto const rotationQuaternion = glm::rotation(glm::vec3(0.f, -1.f, 0.f), glm::normalize(transformAttributes._velocity));

    auto modelMatrix = glm::translate(glm::mat4{1}, transformAttributes._position);
    modelMatrix      = modelMatrix * glm::mat4_cast(rotationQuaternion);
    modelMatrix      = glm::scale(modelMatrix, glm::vec3{_radius});

    switch (renderingDatas._renderType)
    {
    case utils::RenderType::Classic:
        _shader._program.use();
        _shader.setMatrices(modelMatrix, renderingDatas);
        break;

    case utils::RenderType::DepthMap:
        _depthMap._program.use();
        _depthMap.setMatrices(modelMatrix, renderingDatas._lightSpaceMatrix);
        break;
    }

    _LODHandler.drawCorrespondingModel(transformAttributes._position);

    glUseProgram(0);
    glCullFace(GL_BACK);
}

} // namespace Rendering::Shapes