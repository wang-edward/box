#pragma once
#include "color.hh"
#include <array>
#include <GLFW/glfw3.h>
#include <queue>

enum class DeviceType {
    EMULATOR,
    HARDWARE
};

class IOInterface {
public:
    static const int WIDTH = 128;
    static const int HEIGHT = 128;

    constexpr static DeviceType DisplayType = DeviceType::EMULATOR;
    constexpr static DeviceType InputType = DeviceType::EMULATOR;

    IOInterface() {
        activeBuffer.fill(Color{0, 0, 0});
        inactiveBuffer.fill(Color{0, 0, 0});

        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        window = glfwCreateWindow(WIDTH * 4, HEIGHT * 4, "128x128 Display", nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glViewport(0, 0, WIDTH * 4, HEIGHT * 4);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, WIDTH, HEIGHT, 0.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        if constexpr (InputType == DeviceType::HARDWARE) {
            // Initialize microcontroller input handling (e.g., serial interface)
        }
    }

    ~IOInterface() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void drawPixel(int x, int y, Color color) {
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
            inactiveBuffer[y * WIDTH + x] = color;
        }
    }

    void swapBuffers() {
        std::swap(activeBuffer, inactiveBuffer);
    }

    void drawToScreen() const {
        glClear(GL_COLOR_BUFFER_BIT);

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                Color color = activeBuffer[y * WIDTH + x];
                glColor3ub(color.r, color.g, color.b);
                glBegin(GL_QUADS);
                glVertex2i(x, y);
                glVertex2i(x + 1, y);
                glVertex2i(x + 1, y + 1);
                glVertex2i(x, y + 1);
                glEnd();
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    bool pollEvent(Event& event) {
        if constexpr (InputType == DeviceType::EMULATOR) {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                event.type = EventType::KeyPress;
                event.key = GLFW_KEY_ESCAPE;
                return true;
            }
            return false;
        } else {
            // Poll input from the microcontroller via serial interface
            // Read the front of the queue from the microcontroller's memory
            // Example placeholder for reading from microcontroller
            if (!microcontrollerEventQueue.empty()) {
                event = microcontrollerEventQueue.front();
                microcontrollerEventQueue.pop();
                return true;
            }
            return false;
        }
    }

    bool shouldClose() const {
        return glfwWindowShouldClose(window);
    }

private:
    GLFWwindow* window;
    std::array<Color, WIDTH * HEIGHT> activeBuffer;
    std::array<Color, WIDTH * HEIGHT> inactiveBuffer;

    // Microcontroller event queue (simulated)
    std::queue<Event> microcontrollerEventQueue;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};
