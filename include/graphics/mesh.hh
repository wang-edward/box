#pragma once

#include <vector>
#include "core/util.hh"
#include "graphics/shader.hh"
#include "graphics/texture.hh"

namespace box {

class Mesh {
public:
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
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

    void Init();
    void Deinit();
};

} // namespace box
