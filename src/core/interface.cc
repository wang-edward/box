#include "core/interface.hh"
#include <iostream>

namespace box {

Interface:: Interface(GLFWwindow *window):
    window_{window},
    texture_shader_{"shader/texture.vert", "shader/texture.frag"}
{

    glGenFramebuffers(1, &framebuffer_);
    glBindFramebuffer(framebuffer_, 0);

    // texture
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // connect them
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_, 0);
    
    // Setup VAO, VBO, and layout
    vao_.Bind();
    vbo_.Bind();
    vbo_.SetData(QUAD_VERTICES, sizeof(QUAD_VERTICES));

    layout_.Push<float>(3); // position
    layout_.Push<float>(2); // texture coordinates
    vao_.AddBuffer(vbo_, layout_);

    if constexpr (INPUT_TYPE == DeviceType::Hardware) {
        // Initialize microcontroller input handling (e.g., serial interface)
    }
}

Interface:: ~Interface() {
    if (DISPLAY_TYPE == DeviceType::Emulator) {
        glDeleteTextures(1, &texture_);
        glDeleteFramebuffers(1, &framebuffer_);
        glfwDestroyWindow(window_);
        glfwTerminate();
    }
}

void Interface:: PrepRender() {
    // Render to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error{"ERROR::FRAMEBUFFER:: Framebuffer is not complete!"};
    }
    glViewport(0, 0, 128, 128); // Render to the size of the texture TODO WIDTH
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // do the rest of the rendering
}

void Interface:: Display() const {
    if (DISPLAY_TYPE == DeviceType::Emulator) {

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // adjust for emu window
        int window_width, window_height;
        glfwGetFramebufferSize(window_, &window_width, &window_height);
        glViewport(0, 0, window_width, window_height);

        // reset stuff

        // setup texture
        texture_shader_.Bind();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_);

        // Draw quad
        vao_.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        vao_.Unbind();

        glfwSwapBuffers(window_);
    // hardware rendering
    // } else if (DISPLAY_TYPE == DeviceType::Hardware) {
        GLubyte* pixels = new GLubyte[128 * 128 * 3]; // 128x128 texture with 3 components (RGB)
        glReadPixels(0, 0, 128, 128, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        for (int i = 0; i < 128*128*3; i++) {
            if (pixels[i] > 0) {
                std::cout << pixels[i] << " ";
            }
        }
        std::cout << std::endl;

        // std::cout << std::to_string(pixels[0]) << " " << std::to_string(pixels[1]) << " " << std::to_string(pixels[2]) << std::endl;
    }
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
