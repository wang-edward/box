#pragma once
#include "core/util.hh"
#include "core/parameter.hh"
#include "graphics/shader.hh"
#include "graphics/mesh.hh"

namespace box {

template <typename T>
class Knob {
public:

    // TODO add min, max, name
    Knob(int x, int y, juce::CachedValue<T> &value, te::AutomatableParameter *ap = nullptr, std::string name = ""):
        x_{x}, y_{y}, param_{value, ap}, name_{name},
        shader_{"shader/texture.vert", "shader/texture.frag"}
    {
        texture_.LoadFromFile("assets/red_texture.png");
    }

    virtual void Render(Interface &interface) {
        float percentage = param_.GetNorm();
        // Calculate end angle based on percentage
        float end_angle = 2.0f * M_PI * percentage;
        const int num_segments = 360;
        const float radius = 0.5f;
        int n = (num_segments + 2) * 5;  // Updated stride to 5 (x, y, z, u, v)
        std::vector<float> circle_vertices(n); // +2 for center and closing point

        // Center vertex (x, y, z, u, v)
        circle_vertices[0] = x_;
        circle_vertices[1] = y_;
        circle_vertices[2] = 0.0f;
        circle_vertices[3] = 0.5f; // Center texture coordinate u
        circle_vertices[4] = 0.5f; // Center texture coordinate v

        // Generate vertices based on percentage
        int vertex_count = 1;
        for (int i = 0; i < num_segments; ++i) {
            float angle = 2.0f * M_PI * (float)i / (float)num_segments;
            if (angle > end_angle) {
                break;
            }
            circle_vertices[vertex_count * 5] = x_ + radius * cosf(angle); // x
            circle_vertices[vertex_count * 5 + 1] = y_ + radius * sinf(angle); // y
            circle_vertices[vertex_count * 5 + 2] = 0.0f; // z
            // Calculate texture coordinates (u, v) based on vertex position
            circle_vertices[vertex_count * 5 + 3] = 0.5f + 0.5f * cosf(angle); // u
            circle_vertices[vertex_count * 5 + 4] = 0.5f + 0.5f * sinf(angle); // v
            vertex_count++;
        }
        // Add the last point to close the filled part
        circle_vertices[vertex_count * 5] = x_;
        circle_vertices[vertex_count * 5 + 1] = y_;
        circle_vertices[vertex_count * 5 + 2] = 0.0f;
        circle_vertices[vertex_count * 5 + 3] = 0.5f; // Center texture coordinate u
        circle_vertices[vertex_count * 5 + 4] = 0.5f; // Center texture coordinate v
        vertex_count++;

        std::vector<unsigned int> circle_indices(vertex_count + 2); // Updated to use vertex_count instead of num_segments
        for (int i = 0; i < vertex_count + 2; i++) {
            circle_indices[i] = i;
        }

        Mesh circle{circle_vertices, circle_indices, Mesh::Primitive::TriangleFan};
        circle.Render(shader_, &texture_);
    }
    virtual void HandleEvent(const Event& event) {

    }

    Parameter<T> param_;
private:
    int x_, y_;
    std::string name_;
    Shader shader_;
    Texture texture_;
};

} // namespace box

