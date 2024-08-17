#include "graphics/framebuffer.hh"

namespace box {


Framebuffer::Framebuffer() {
    glGenFramebuffers(1, &framebufferID_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID_);
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &framebufferID_);
}

void Framebuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID_);
}

void Framebuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::AttachTexture(const Texture& texture, GLenum attachmentType) {
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, texture.GetTarget(), texture.GetID(), 0);
    Unbind();
}

void Framebuffer::AssertComplete() const {
    Bind();
    bool complete = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    if (complete == false) {
        throw std::runtime_error{"Framebuffer: " + std::to_string(framebufferID_) + " not complete"};
    }
    Unbind();
}


} // namespace box
