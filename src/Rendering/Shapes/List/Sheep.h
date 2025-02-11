#pragma once

#include <p6/p6.h>
#include <cstddef> // For offsetof()
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Rendering/Engine/Mesh.h"
#include "Rendering/Engine/Model.h"
#include "Rendering/Geometries/geometriesVertices.hpp"
#include "Rendering/Programs/DepthMap/DepthMap.h"
#include "Rendering/Programs/ShadyToon/ToonAndShadow.h"
#include "utils/LODHandler.h"
#include "utils/RenderingDatas.h"
#include "utils/TransformAttributes.h"

namespace Rendering::Shapes {

class Sheep {
public:
    explicit Sheep();

    // We delete the copy constructors because the class has resources that cannot be copied
    Sheep(const Sheep& other)            = delete;
    Sheep& operator=(const Sheep& other) = delete;

    // Move constructors
    Sheep(Sheep&& other) noexcept            = default;
    Sheep& operator=(Sheep&& other) noexcept = default;

    void draw(utils::TransformAttributes const& transformAttributes, utils::RenderingDatas&) const;

private:
    Programs::ToonAndShadow _shader{};
    Programs::DepthMap      _depthMap{};

    Engine::Model _model;
};

} // namespace Rendering::Shapes