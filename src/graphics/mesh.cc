#include "graphics/Mesh.hh"

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) 
    : index_count_(indices.size()) {
    // vao
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // vbo
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // ebo
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position (3 floats) and texture (2 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // unbind vao
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &ebo_);
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
}

void Mesh::Bind() const {
    glBindVertexArray(vao_);
}

void Mesh::Unbind() const {
    glBindVertexArray(0);
}

void Mesh::Render() const {
    Bind();
    glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, 0);
    Unbind();
}
