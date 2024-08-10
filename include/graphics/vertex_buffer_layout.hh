#pragma once
#include "core/util.hh"
#include <vector>

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT: return sizeof(GLfloat);
            case GL_UNSIGNED_INT: return sizeof(GLuint);
            case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
            // TODO more cases
        }
        assert(false);
        return 0;
    }
};

class VertexBufferLayout {
public:
    VertexBufferLayout()
        : stride_(0) {}

    template<typename T>
    void Push(unsigned int count) {
        (void) count;
        throw std::runtime_error{"VertexBufferLayout::Push called with bad type"};
    }

    template<>
    void Push<float>(unsigned int count) {
        elements_.push_back({GL_FLOAT, count, GL_FALSE});
        stride_ += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    template<>
    void Push<unsigned int>(unsigned int count) {
        elements_.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        stride_ += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void Push<unsigned char>(unsigned int count) {
        elements_.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        stride_ += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    inline const std::vector<VertexBufferElement>& GetElements() const { return elements_; }
    inline unsigned int GetStride() const { return stride_; }

private:
    std::vector<VertexBufferElement> elements_;
    unsigned int stride_;
};
