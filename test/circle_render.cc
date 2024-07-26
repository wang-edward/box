// circle_render.cc

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Define the size of the array
const int WIDTH = 128;
const int HEIGHT = 128;

GLFWwindow* initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // Create an invisible window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Offscreen", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return nullptr;
    }

    return window;
}

GLuint createFramebuffer(GLuint texture) {
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!\n";
        return 0;
    }

    return framebuffer;
}

void renderCircle(int width, int height) {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glVertex2f(width / 2, height / 2); // Center of circle
    for (int angle = 0; angle <= 360; angle += 1) {
        float rad = angle * 3.14159f / 180.0f;
        glVertex2f(width / 2 + cos(rad) * width / 4, height / 2 + sin(rad) * height / 4);
    }
    glEnd();
}

void readPixelsToArray(int width, int height, unsigned char* buffer) {
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);
}

int main() {
    GLFWwindow* window = initOpenGL();
    if (!window) return -1;

    // Create a texture to render to
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLuint framebuffer = createFramebuffer(texture);
    if (!framebuffer) return -1;

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    renderCircle(WIDTH, HEIGHT);

    unsigned char buffer[WIDTH * HEIGHT * 3]; // Array to store the pixel data
    readPixelsToArray(WIDTH, HEIGHT, buffer);

    // The buffer now contains the pixel data in RGB format
    // Print the RGB values of the center pixel as a test
    int centerIndex = (WIDTH / 2 + HEIGHT / 2 * WIDTH) * 3;
    std::cout << "Center pixel RGB: ("
              << static_cast<int>(buffer[centerIndex]) << ", "
              << static_cast<int>(buffer[centerIndex + 1]) << ", "
              << static_cast<int>(buffer[centerIndex + 2]) << ")\n";

    glDeleteFramebuffers(1, &framebuffer);
    glDeleteTextures(1, &texture);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
