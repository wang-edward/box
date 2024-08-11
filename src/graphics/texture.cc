#include "graphics/texture.hh"

Texture::Texture(GLenum target) : target_(target) {
    glGenTextures(1, &textureID_);
    glBindTexture(target_, textureID_);
    // Set default filtering and wrapping options
    SetFiltering(GL_LINEAR, GL_LINEAR);
    SetWrapping(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    glBindTexture(target_, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &textureID_);
}

void Texture::Bind(GLenum unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(target_, textureID_);
}

void Texture::Unbind() const {
    glBindTexture(target_, 0);
}

void Texture::SetData(int width, int height, GLenum format, GLenum type, const void* data) const {
    Bind();
    glTexImage2D(target_, 0, format, width, height, 0, format, type, data);
    Unbind();
}

void Texture::SetFiltering(GLenum minFilter, GLenum magFilter) const {
    Bind();
    glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, magFilter);
    Unbind();
}

void Texture::SetWrapping(GLenum wrapS, GLenum wrapT) const {
    Bind();
    glTexParameteri(target_, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(target_, GL_TEXTURE_WRAP_T, wrapT);
    Unbind();
}
