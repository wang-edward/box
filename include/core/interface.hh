#pragma once
#include <array>
#include "core/util.hh"
#include "graphics/shader.hh"
#include "graphics/vertex_array.hh"
#include "graphics/vertex_buffer.hh"

namespace box {

class Interface {
public:
    static const int WIDTH = 128;
    static const int HEIGHT = 128;
    constexpr static DeviceType DISPLAY_TYPE = DeviceType::Emulator;
    constexpr static DeviceType INPUT_TYPE = DeviceType::Emulator;

    Interface();
    ~Interface();

    void Render();
    void DrawToScreen() const;

    bool PollEvent(Event& event);
    bool ShouldClose() const;

private:
    GLFWwindow* window_;
    GLuint texture_;
    GLuint framebuffer_;
    Shader texture_shader_;

    std::array<size_t, GLFW_KEY_LAST + 1> key_states_{};
};

} // namespace box
