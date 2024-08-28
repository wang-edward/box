#pragma once

#include <vector>
#include "core/util.hh"
#include "graphics/shader.hh"
#include "graphics/texture.hh"

namespace box {

class Mesh {

public:
    enum class Primitive {
        Triangles = GL_TRIANGLES,
        TriangleFan = GL_TRIANGLE_FAN,
        TriangleStrip = GL_TRIANGLE_STRIP,
        Lines = GL_LINES,
        LineStrip = GL_LINE_STRIP,
        LineLoop = GL_LINE_LOOP,
        Points = GL_POINTS
    };

    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, Primitive primitive);
    ~Mesh();
    Mesh(const Mesh& other);
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh other);
    Mesh& operator=(Mesh&& other) noexcept;

    friend void swap(Mesh& first, Mesh& second) noexcept;

    void Render(const Shader& shader, const Texture* texture = nullptr) const;

private:
    GLuint vao_, vbo_, ebo_;
    std::vector<float> vertices_;
    std::vector<unsigned int> indices_;
    Primitive primitive_;

    void Init();
    void Deinit();
};

} // namespace box
