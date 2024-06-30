#include "interface.hh"
#include <iostream>

Interface:: Interface() {
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
    glfwInit();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glViewport(0, 0, WIDTH * 4, HEIGHT * 4);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if constexpr (InputType == DeviceType::HARDWARE) {
        // Initialize microcontroller input handling (e.g., serial interface)
    }
}

Interface:: ~Interface() {
    if (DisplayType == DeviceType::EMULATOR) {
        glDeleteTextures(1, &texture);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

void Interface::drawPixel(int x, int y, Color color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        inactiveBuffer[y * WIDTH + x] = color;
    }
}

void Interface::swapBuffers() { std::swap(activeBuffer, inactiveBuffer); }

void Interface:: drawToScreen() const {
    glClear(GL_COLOR_BUFFER_BIT);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, activeBuffer.data());

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    int windowWidth, windowHeight;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

    float aspectRatio = float(windowWidth) / float(windowHeight);
    float scale = std::min(float(windowWidth) / WIDTH, float(windowHeight) / HEIGHT);

    float scaledWidth = WIDTH * scale;
    float scaledHeight = HEIGHT * scale;

    float xOffset = (windowWidth - scaledWidth) / 2;
    float yOffset = (windowHeight - scaledHeight) / 2;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(xOffset, yOffset);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(xOffset + scaledWidth, yOffset);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(xOffset + scaledWidth, yOffset + scaledHeight);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(xOffset, yOffset + scaledHeight);
    glEnd();

    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Interface::pollEvent(Event &event) {
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

bool Interface::shouldClose() const { return glfwWindowShouldClose(window); }
