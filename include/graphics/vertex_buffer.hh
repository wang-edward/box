#pragma once
#include "core/util.hh"
class VertexBuffer {
public:
    VertexBuffer() {
        glGenBuffers(1, &buffer_id_);
    }

    VertexBuffer(const void *data, unsigned int size) {
        glGenBuffers(1, &buffer_id_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
    }

    void Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void SetData(const void *data, unsigned int size) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    ~VertexBuffer() {
        glDeleteBuffers(1, &buffer_id_);
    }

private:
    unsigned int buffer_id_;
};
