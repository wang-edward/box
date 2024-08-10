#pragma once
#include "core/util.hh"
#include "vertex_buffer.hh"

class VertexArray {
public:
    VertexArray() {
        glGenVertexArrays(1, &arrayID);
        glBindVertexArray(arrayID);
    }

    void Bind() const {
        glBindVertexArray(arrayID);
    }

    void Unbind() const {
        glBindVertexArray(0);
    }

    void AddBuffer(const VertexBuffer& vb, unsigned int index) {
        Bind();
        vb.Bind();
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }

    ~VertexArray() {
        glDeleteVertexArrays(1, &arrayID);
    }

private:
    unsigned int arrayID;
};
