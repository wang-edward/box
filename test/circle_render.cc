#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <cmath>
#include <iostream>

// Function to check for shader compilation errors
void checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER-COMPILATION-ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "| ERROR::PROGRAM-LINKING-ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

// Vertex Shader source code
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(position, 1.0);
    TexCoord = texCoord;
}
)";

// Fragment Shader source code
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D screenTexture;

void main() {
    FragColor = texture(screenTexture, TexCoord);
}
)";

// Fragment Shader for rendering a red circle
const char* circleFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
}
)";

GLFWwindow *initializeOpenGL() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Request an OpenGL 3.3 context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Window", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set up viewport
    glViewport(0, 0, 800, 800);

    // Disable antialiasing
    glDisable(GL_MULTISAMPLE);
    return window;
}

void updateQuadVertices(float* quadVertices, float scale, float xOffset, float yOffset) {
    // Top-left
    quadVertices[0] = -scale + xOffset; quadVertices[1] = scale + yOffset;
    // Bottom-left
    quadVertices[5] = -scale + xOffset; quadVertices[6] = -scale + yOffset;
    // Bottom-right
    quadVertices[10] = scale + xOffset; quadVertices[11] = -scale + yOffset;
    // Top-left
    quadVertices[15] = -scale + xOffset; quadVertices[16] = scale + yOffset;
    // Bottom-right
    quadVertices[20] = scale + xOffset; quadVertices[21] = -scale + yOffset;
    // Top-right
    quadVertices[25] = scale + xOffset; quadVertices[26] = scale + yOffset;
}

void renderScene(GLuint circleVAO, GLuint circleShaderProgram, float percentage) {
    // Render your scene here
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render red circle
    glUseProgram(circleShaderProgram);
    glBindVertexArray(circleVAO);

    // Calculate end angle based on percentage
    float endAngle = 2.0f * M_PI * percentage;
    const int numSegments = 360;
    const float radius = 0.5f;
    float circleVertices[(numSegments + 2) * 3]; // +2 for center and closing point

    // Center vertex
    circleVertices[0] = 0.0f;
    circleVertices[1] = 0.0f;
    circleVertices[2] = 0.0f;

    // Generate vertices based on percentage
    int vertexCount = 1;
    for (int i = 0; i <= numSegments; ++i) {
        float angle = 2.0f * M_PI * (float)i / (float)numSegments;
        if (angle > endAngle) {
            break;
        }
        circleVertices[vertexCount * 3] = radius * cosf(angle);
        circleVertices[vertexCount * 3 + 1] = radius * sinf(angle);
        circleVertices[vertexCount * 3 + 2] = 0.0f;
        vertexCount++;
    }
    // Add the last point to close the filled part
    circleVertices[vertexCount * 3] = 0.0f;
    circleVertices[vertexCount * 3 + 1] = 0.0f;
    circleVertices[vertexCount * 3 + 2] = 0.0f;
    vertexCount++;

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount); // Draw filled circle segment
    glDeleteBuffers(1, &VBO);

    glBindVertexArray(0);
}

int main() {
    auto window = initializeOpenGL();

    // Shader compilation
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Circle shader compilation
    GLuint circleFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(circleFragmentShader, 1, &circleFragmentShaderSource, NULL);
    glCompileShader(circleFragmentShader);
    checkCompileErrors(circleFragmentShader, "FRAGMENT");

    GLuint circleShaderProgram = glCreateProgram();
    glAttachShader(circleShaderProgram, vertexShader); // Reuse vertex shader
    glAttachShader(circleShaderProgram, circleFragmentShader);
    glLinkProgram(circleShaderProgram);
    checkCompileErrors(circleShaderProgram, "PROGRAM");

    glDeleteShader(circleFragmentShader);

    // Setup FBO
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Create a texture to render to
    GLuint textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Quad vertices
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f
    };

    // Setup quad VAO
    GLuint quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Setup circle VAO
    GLuint circleVAO;
    glGenVertexArrays(1, &circleVAO);
    glBindVertexArray(circleVAO);
    glBindVertexArray(0);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Render to framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, 128, 128); // Render to the size of the texture
        renderScene(circleVAO, circleShaderProgram, 0.75f); // Example percentage
        {
            // Allocate memory to store the pixel data
            GLubyte* pixels = new GLubyte[128 * 128 * 3]; // 128x128 texture with 3 components (RGB)

            // Read the pixels from the framebuffer
            glReadPixels(64, 64, 128, 128, GL_RGB, GL_UNSIGNED_BYTE, pixels);
            // std::cout << std::to_string(pixels[0]) << " " << std::to_string(pixels[1]) << " " << std::to_string(pixels[2]) << std::endl;
        }

        // Render to screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        int window_width, window_height;
        glfwGetFramebufferSize(window, &window_width, &window_height);

        {
            float scale = std::min(window_width, window_height) / static_cast<float>(std::max(window_width, window_height));
            float xOffset = 0.0f, yOffset = 0.0f;

            if (window_width > window_height) {
                xOffset = (1.0f - scale) / 2.0f;
            } else {
                yOffset = (1.0f - scale) / 2.0f;
            }

            updateQuadVertices(quadVertices, scale, xOffset, yOffset);

                glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
        }


        glViewport(0, 0, window_width, window_height);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the quad with the texture
        glUseProgram(shaderProgram);
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &circleVAO);
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteTextures(1, &textureColorbuffer);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
