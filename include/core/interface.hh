#pragma once
#include <array>
#include "core/util.hh"
#include "graphics/shader.hh"
#include "graphics/framebuffer.hh"
#include "graphics/texture.hh"
#include "graphics/mesh.hh"

namespace box {

class Interface {
public:
    static const int WIDTH = 128;
    static const int HEIGHT = 128;
    constexpr static DeviceType DISPLAY_TYPE = DeviceType::Emulator;
    constexpr static DeviceType INPUT_TYPE = DeviceType::Emulator;

    Interface(GLFWwindow *window);
    ~Interface();

    void PrepRender();
    void Display() const;

    bool PollEvent(Event& event);
    bool ShouldClose() const;

private:
    GLFWwindow* window_;
    Texture pixel_tex_;
    Framebuffer pixel_buffer_;
    Shader pixel_shader_; 
    Mesh pixel_quad_;
    
    std::array<size_t, GLFW_KEY_LAST + 1> key_states_{};

};

} // namespace box
