#include "interface.hh"
#include <iostream>

Interface::Interface() {
    activeBuffer.fill(Color{0, 0, 0});
    inactiveBuffer.fill(Color{0, 0, 0});

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    window = glfwCreateWindow(WIDTH * 4, HEIGHT * 4, "128x128 Display", nullptr,
                              nullptr);
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

Interface::~Interface() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Interface::drawPixel(int x, int y, Color color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        inactiveBuffer[y * WIDTH + x] = color;
    }
}

void Interface:: swapBuffers() {
    std::swap(activeBuffer, inactiveBuffer);
}

void Interface:: drawToScreen() const {
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

bool Interface:: pollEvent(Event& event) {
    if constexpr (InputType == DeviceType::EMULATOR) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            event.type = EventType::KEYPRESS;
            event.value = GLFW_KEY_ESCAPE;
            return true;
        }
        return false;
    } else {
        // mcu stuff
        return false;
    }
}

bool Interface:: shouldClose() const {
    return glfwWindowShouldClose(window);
}
