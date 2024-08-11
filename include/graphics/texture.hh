#pragma once

#include "core/util.hh"

class Texture {
public:
    Texture(GLenum target = GL_TEXTURE_2D);
    ~Texture();

    void Bind(GLenum unit = 0) const;
    void Unbind() const;
    
    void SetData(int width, int height, GLenum format = GL_RGB, GLenum type = GL_UNSIGNED_BYTE, const void* data = nullptr) const;
    void SetFiltering(GLenum minFilter, GLenum magFilter) const;
    void SetWrapping(GLenum wrapS, GLenum wrapT) const;

    GLuint GetID() const { return textureID_; }
    GLuint GetTarget() const { return target_; }

private:
    GLuint textureID_;
    GLenum target_;
};
