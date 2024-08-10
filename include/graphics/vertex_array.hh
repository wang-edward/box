#pragma once
#include "core/util.hh"
#include "vertex_buffer.hh"
#include "vertex_buffer_layout.hh"

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

    // void AddBuffer(const VertexBuffer& vb, unsigned int index) {
    //     Bind();
    //     vb.Bind();
    //     glEnableVertexAttribArray(index);
    //     glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // }

    void AddBuffer(const VertexBuffer &vb, VertexBufferLayout &layout) {
        Bind();
        vb.Bind();
        const auto& elements = layout.GetElements();
        unsigned int offset = 0;
        for (unsigned int i = 0; i < elements.size(); i++) {
            const auto& element = elements[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
            offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
        }
    }

    ~VertexArray() {
        glDeleteVertexArrays(1, &arrayID);
    }

private:
    unsigned int arrayID;
};
