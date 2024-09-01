#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "graphics/texture.hh"

namespace box {


Texture::Texture(GLenum target) : target_(target) {
    glGenTextures(1, &textureID_);
    glBindTexture(target_, textureID_);
    // Set default filtering and wrapping options
    SetFiltering(GL_NEAREST, GL_NEAREST);
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

void Texture::LoadFromFile(const std::string& filepath) {
    // Load image using stb_image
    int channels;
    stbi_set_flip_vertically_on_load(true); // Flip the image vertically to match OpenGL's coordinate system
    unsigned char* data = stbi_load(filepath.c_str(), &width_, &height_, &channels, 0);
    
    if (!data) {
        throw std::runtime_error{"Failed to load texture: " + filepath};
    }

    // Determine the format based on the number of channels
    GLenum format;
    if (channels == 1)
        format = GL_RED;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 4)
        format = GL_RGBA;

    // Set the texture data
    SetData(width_, height_, format, GL_UNSIGNED_BYTE, data);

    // Free the image memory
    stbi_image_free(data);
}


} // namespace box
