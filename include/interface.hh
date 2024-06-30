#pragma once
#include "util.hh"
#include <array>
#include <GLFW/glfw3.h>
#include <queue>

class Interface {
public:
    static const int WIDTH = 128;
    static const int HEIGHT = 128;

    constexpr static DeviceType DisplayType = DeviceType::EMULATOR;
    constexpr static DeviceType InputType = DeviceType::EMULATOR;

    Interface();
    
    ~Interface();

    void drawPixel(int x, int y, Color color);

    void swapBuffers();

    void drawToScreen() const;

    bool pollEvent(Event& event);
    bool shouldClose() const;

private:
    GLFWwindow* window;
    GLuint texture;
    std::array<Color, WIDTH * HEIGHT> activeBuffer;
    std::array<Color, WIDTH * HEIGHT> inactiveBuffer;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};
