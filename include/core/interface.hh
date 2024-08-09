#pragma once
#include <array>
#include "core/util.hh"

namespace box {

class Interface {
public:
    static const int WIDTH = 128;
    static const int HEIGHT = 128;
    constexpr static DeviceType DISPLAY_TYPE = DeviceType::Emulator;
    constexpr static DeviceType INPUT_TYPE = DeviceType::Emulator;

    Interface();
    ~Interface();

    void Clear();

    bool PollEvent(Event& event);
    bool ShouldClose() const;

private:
    GLFWwindow* window_;
    GLuint texture_;
    GLuint framebuffer_;

    std::array<size_t, GLFW_KEY_LAST + 1> key_states_{};

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};

} // namespace box
