#pragma once

#include <GL/glew.h>
#include <vector>

class Mesh {
public:
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();

    void Bind() const;
    void Unbind() const;

    void Render() const;

private:
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;
    GLsizei index_count_;
};
