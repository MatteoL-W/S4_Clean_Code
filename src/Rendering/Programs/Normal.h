#pragma once

#include "p6/p6.h"

namespace Rendering::Programs {

struct Normal {
    p6::Shader _program;

    GLint uMVPMatrix{};
    GLint uMVMatrix{};
    GLint uNormalMatrix{};

    Normal()
        : _program{p6::load_shader("../src/Rendering/Programs/Shaders/3D.vs.glsl", "../src/Rendering/Programs/Shaders/normal.fs.glsl")}
        , uMVPMatrix(glGetUniformLocation(_program.id(), "uMVPMatrix"))
        , uMVMatrix(glGetUniformLocation(_program.id(), "uMVMatrix"))
        , uNormalMatrix(glGetUniformLocation(_program.id(), "uNormalMatrix"))
    {}
};

} // namespace Rendering::Programs