#include "core/interface.hh"
#include <iostream>

namespace box {

Interface:: Interface(GLFWwindow *window):
    window_{window},
    pixel_shader_{"shader/texture.vert", "shader/texture.frag"},
    pixel_quad_{
        {
            // Positions      // Texture Coords
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,   // Top-left
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,   // Bottom-left
             1.0f, -1.0f, 0.0f,  1.0f, 0.0f,   // Bottom-right
             1.0f,  1.0f, 0.0f,  1.0f, 1.0f    // Top-right
        }, 
        {
            0, 1, 2,  // First triangle (Top-left, Bottom-left, Bottom-right)
            2, 3, 0   // Second triangle (Bottom-right, Top-right, Top-left)
        },
        Mesh::Primitive::Triangles
    }
{
    pixel_buffer_.Bind();

    pixel_tex_.SetData(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    pixel_tex_.SetFiltering(GL_NEAREST, GL_NEAREST);

    // connect
    pixel_buffer_.AttachTexture(pixel_tex_);
    pixel_buffer_.AssertComplete();
    
    if constexpr (INPUT_TYPE == DeviceType::Hardware) {
        // Initialize microcontroller input handling (e.g., serial interface)
    }
}

Interface:: ~Interface() {
    if (DISPLAY_TYPE == DeviceType::Emulator) {
        glfwDestroyWindow(window_);
        glfwTerminate();
    }
}

void Interface:: PrepRender() {
    // Render to framebuffer
    pixel_buffer_.Bind();
    glViewport(0, 0, WIDTH, HEIGHT); // Render to the size of the texture TODO WIDTH
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

        // setup texture
        pixel_shader_.Bind();
        pixel_tex_.Bind();

        // Draw quad
        pixel_quad_.Render(pixel_shader_, &pixel_tex_);

        glfwSwapBuffers(window_);
    // hardware rendering
    } else if (DISPLAY_TYPE == DeviceType::Hardware) {
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
