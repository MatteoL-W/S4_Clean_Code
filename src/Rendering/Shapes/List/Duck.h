#pragma once

#include <p6/p6.h>
#include <cstddef> // For offsetof()
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Rendering/Engine/Mesh.h"
#include "Rendering/Engine/Model.h"
#include "Rendering/Geometries/geometriesVertices.hpp"
#include "Rendering/Programs/DepthMap/DepthMap.h"
#include "Rendering/Programs/ShadyPhong/PhongAndShadow.h"
#include "utils/RenderingDatas.h"
#include "utils/TransformAttributes.h"

namespace Rendering::Shapes {

class Duck {
public:
    explicit Duck();

    // We delete the copy constructors because the class has resources that cannot be copied
    Duck(const Duck& other)            = delete;
    Duck& operator=(const Duck& other) = delete;

    // Move constructors
    Duck(Duck&& other) noexcept            = default;
    Duck& operator=(Duck&& other) noexcept = default;

public:
    void  draw(utils::TransformAttributes const& transformAttributes, utils::RenderingDatas&) const;
    void  setRadius(float radius) { _radius = radius; };
    float getRadius() const { return _radius; };

private:
    float                               _radius{};
    Rendering::Programs::PhongAndShadow _shader{};
    Rendering::Programs::DepthMap       _depthMap{};

    Model                               _HQModel;
    Model                               _MQModel;
    Model                               _LQModel;
};

} // namespace Rendering::Shapes