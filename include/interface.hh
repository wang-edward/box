#pragma once
#include "util.hh"
#include <array>
#include <GLFW/glfw3.h>
#include <queue>

namespace box {

class Interface {
public:
    static const int WIDTH = 128;
    static const int HEIGHT = 128;

    constexpr static DeviceType DISPLAY_TYPE = DeviceType::Emulator;
    constexpr static DeviceType INPUT_TYPE = DeviceType::Emulator;

    Interface();
    
    ~Interface();

    void draw_pixel(int x, int y, Color color);

    void swap_buffers();

    void draw_to_screen() const;

    bool poll_event(Event& event);
    bool should_close() const;

private:
    GLFWwindow* window_;
    GLuint texture_;
    std::array<Color, WIDTH * HEIGHT> active_buffer_;
    std::array<Color, WIDTH * HEIGHT> inactive_buffer_;

    std::array<int, GLFW_KEY_LAST + 1> key_states_{};

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};

} // namespace box
