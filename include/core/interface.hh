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

    void DrawPixel(int x, int y, Color color);

    void SwapBuffers();

    void DrawToScreen() const;

    bool PollEvent(Event& event);
    bool ShouldClose() const;

private:
    GLFWwindow* window_;
    GLuint texture_;
    std::array<Color, WIDTH * HEIGHT> active_buffer_;
    std::array<Color, WIDTH * HEIGHT> inactive_buffer_;

    std::array<size_t, GLFW_KEY_LAST + 1> key_states_{};

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};

} // namespace box
