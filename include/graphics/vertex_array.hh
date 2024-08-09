#include "core/util.hh"

class VertexArray {
public:
    VertexArray() {
        glGenVertexArrays(1, &arrayID);
        glBindVertexArray(arrayID);
    }

    void bind() const {
        glBindVertexArray(arrayID);
    }

    void unbind() const {
        glBindVertexArray(0);
    }

    void addBuffer(const VertexBuffer& vb, unsigned int index) {
        bind();
        vb.bind();
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }

    ~VertexArray() {
        glDeleteVertexArrays(1, &arrayID);
    }

private:
    unsigned int arrayID;
};
