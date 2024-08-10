#pragma once
#include "core/parameter.hh"
#include "graphics/shader.hh"
#include "graphics/vertex_buffer_layout.hh"

namespace box {

template <typename T>
class Knob {
public:

    // TODO add min, max, name
    Knob(int x, int y, juce::CachedValue<T> &value, te::AutomatableParameter *ap = nullptr, std::string name = ""):
        x_{x}, y_{y}, param_{value, ap}, name_{name}, 
        shader_{"shader/texture.vert", "shader/red.frag"} {}

    virtual void Render(Interface &interface) {
        float percentage = param_.GetNorm();
        // Render your scene here
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render red circle
        shader_.Bind();
        circle_vao_.Bind();

        // Calculate end angle based on percentage
        float end_angle = 2.0f * M_PI * percentage;
        const int num_segments = 360;
        const float radius = 0.5f;
        float circle_vertices[(num_segments + 2) * 3]; // +2 for center and closing point

        // Center vertex
        circle_vertices[0] = 0.0f;
        circle_vertices[1] = 0.0f;
        circle_vertices[2] = 0.0f;

        // Generate vertices based on percentage
        int vertex_count = 1;
        for (int i = 0; i < num_segments; ++i) {
            float angle = 2.0f * M_PI * (float)i / (float)num_segments;
            if (angle > end_angle) {
                break;
            }
            circle_vertices[vertex_count * 3] = radius * cosf(angle);
            circle_vertices[vertex_count * 3 + 1] = radius * sinf(angle);
            circle_vertices[vertex_count * 3 + 2] = 0.0f;
            vertex_count++;
        }
        // Add the last point to close the filled part
        circle_vertices[vertex_count * 3] = 0.0f;
        circle_vertices[vertex_count * 3 + 1] = 0.0f;
        circle_vertices[vertex_count * 3 + 2] = 0.0f;
        vertex_count++;

        VertexBuffer vbo{circle_vertices, sizeof(circle_vertices)};
        VertexBufferLayout layout;
        layout.Push<float>(3);
        circle_vao_.AddBuffer(vbo, layout);

        glDrawArrays(GL_TRIANGLE_FAN, 0, vertex_count); // Draw filled circle segment

        glBindVertexArray(0);
    }
    virtual void HandleEvent(const Event& event) {

    }

    Parameter<T> param_;
private:
    int x_, y_;
    std::string name_;
    Shader shader_;
    VertexArray circle_vao_;
};

} // namespace box

