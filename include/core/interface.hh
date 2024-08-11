#pragma once
#include <array>
#include "core/util.hh"
#include "graphics/shader.hh"
#include "graphics/vertex_array.hh"
#include "graphics/vertex_buffer.hh"
#include "graphics/framebuffer.hh"
#include "graphics/texture.hh"

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
    Texture texture_;
    Framebuffer framebuffer_;

    // need optional for this because glfw has to be initted first
    Shader texture_shader_; 
    VertexArray vao_;
    VertexBuffer vbo_;
    VertexBufferLayout layout_;

    std::array<size_t, GLFW_KEY_LAST + 1> key_states_{};

    static constexpr float QUAD_VERTICES[6 * 5] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f,   1.0f, 0.0f,  1.0f, 1.0f
    };
};

} // namespace box
