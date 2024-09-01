#include "graphics/mesh.hh"
#include <iostream>

namespace box {


// Mesh::Mesh() : vao_{0}, vbo_{0}, ebo_{0} {}

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, Primitive primitive = Primitive::Triangles)
    : vao_{0}, vbo_{0}, ebo_{0}, vertices_{vertices}, indices_{indices},
    primitive_{primitive} {
    // Generate and bind the VAO
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // Generate and bind the VBO
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float), vertices_.data(), GL_STATIC_DRAW);

    // Generate and bind the EBO (if you are using indices)
    if (!indices_.empty()) {
        glGenBuffers(1, &ebo_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);
    }

    // Position attribute (location = 0 in the shader)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute (location = 1 in the shader)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind the VAO (not the EBO, as the VAO stores its binding)
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    if (vao_ != 0) {
        glDeleteVertexArrays(1, &vao_);
    }
    if (vbo_ != 0) {
        glDeleteBuffers(1, &vbo_);
    }
    if (ebo_ != 0) {
        glDeleteBuffers(1, &ebo_);
    }
}

void Mesh::Render(const Shader& shader, const Texture* texture) const {
    // Bind the VAO
    glBindVertexArray(vao_);

    // Set the render mode uniform in the shader
    shader.Bind();

    // Set the appropriate shader uniforms based on the render mode
    texture->Bind();

    // Draw the mesh using indices
    if (!indices_.empty()) {
        glDrawElements(static_cast<GLenum>(primitive_), static_cast<GLsizei>(indices_.size()), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(static_cast<GLenum>(primitive_), 0, static_cast<GLsizei>(vertices_.size() / 5)); // Adjust based on your vertex structure
    }

    // Unbind the VAO
    glBindVertexArray(0);
}


} // namespace box
