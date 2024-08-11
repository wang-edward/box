#pragma once

#include "core/util.hh"
#include "graphics/texture.hh"

class Framebuffer {
public:
    Framebuffer();
    ~Framebuffer();

    void Bind() const;
    void Unbind() const;

    void AttachTexture(const Texture& texture, GLenum attachmentType = GL_COLOR_ATTACHMENT0);
    void AssertComplete() const;

private:
    GLuint framebufferID_;
};
