#include "core/interface.hh"
#include <iostream>

namespace box {

Interface:: Interface() {
    active_buffer_.fill(Color{0, 0, 0});
    inactive_buffer_.fill(Color{0, 0, 0});

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    window_ = glfwCreateWindow(WIDTH * 4, HEIGHT * 4, "128x128 Display", nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window_);
    glfwInit();
    glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);
    // glViewport(0, 0, WIDTH * 4, HEIGHT * 4);

    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if constexpr (INPUT_TYPE == DeviceType::Hardware) {
        // Initialize microcontroller input handling (e.g., serial interface)
    }
}

Interface:: ~Interface() {
    if (DISPLAY_TYPE == DeviceType::Emulator) {
        glDeleteTextures(1, &texture_);
        glfwDestroyWindow(window_);
        glfwTerminate();
    }
}

void Interface::SwapBuffers() { std::swap(active_buffer_, inactive_buffer_); }

void Interface:: DrawToScreen() const {
    glClear(GL_COLOR_BUFFER_BIT);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, active_buffer_.data());

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_);

    int window_width, window_height;
    glfwGetFramebufferSize(window_, &window_width, &window_height);

    float aspect_ratio = float(window_width) / float(window_height);
    float scale = std::min(float(window_width) / WIDTH, float(window_height) / HEIGHT);

    float scaled_width = WIDTH * scale;
    float scaled_height = HEIGHT * scale;

    float x_offset = (window_width - scaled_width) / 2;
    float y_offset = (window_height - scaled_height) / 2;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, window_width, window_height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(x_offset, y_offset);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(x_offset + scaled_width, y_offset);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(x_offset + scaled_width, y_offset + scaled_height);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(x_offset, y_offset + scaled_height);
    glEnd();

    glfwSwapBuffers(window_);
}

void Interface::DrawPixel(int x, int y, Color color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        inactive_buffer_[y * WIDTH + x] = color;
    }
}

void Interface::Clear() {
    inactive_buffer_.fill(Color{0, 0, 0});
}

bool Interface:: PollEvent(Event& event) {
    glfwPollEvents();

    for (int value = GLFW_KEY_SPACE; value <= GLFW_KEY_LAST; ++value) {
        int state = glfwGetKey(window_, value);
        if (state == GLFW_PRESS) {
            if (key_states_[value] == GLFW_RELEASE) {
                event.type = EventType::KeyPress;
                event.value = value;
                key_states_[value] = state;
                return true;
            }
        } else if (state == GLFW_RELEASE) {
            if (key_states_[value] == GLFW_PRESS) {
                event.type = EventType::KeyRelease;
                event.value = value;
                key_states_[value] = state;
                return true;
            }
        }
    }
    return false;
}

bool Interface::ShouldClose() const { return glfwWindowShouldClose(window_); }

} // namespace box
