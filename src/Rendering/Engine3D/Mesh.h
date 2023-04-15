#pragma once

#include "Rendering/Geometries/Vertex3D.hpp"
#include "p6/p6.h"

namespace RenderEngine {

class Mesh {
public:
    explicit Mesh(std::vector<Rendering::Geometries::Vertex3D>);
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
    ~Mesh();

    GLuint getVbo() const { return _vbo; };
    GLuint getVao() const { return _vao; };

private:
    void generateVbo(std::vector<Rendering::Geometries::Vertex3D> vertices);
    void generateVao();

private:
    const GLuint VERTEX_ATTR_POSITION   = 0;
    const GLuint VERTEX_ATTR_NORMAL     = 1;
    const GLuint VERTEX_ATTR_TEX_COORDS = 2;

    GLuint _vbo = 0;
    GLuint _vao = 0;
};

} // namespace RenderEngine
