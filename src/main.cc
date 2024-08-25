#include <random>
#include <iostream>

#include <juce_events/juce_events.h>

#include "core/util.hh"
#include "core/interface.hh"
#include "core/track_manager.hh"
#include "core/manager.hh"
#include "plugin/four_osc.hh"
#include "plugin/delay.hh"

int main() {
    const juce::ScopedJuceInitialiser_GUI initialiser;

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(box::Interface::WIDTH * 4, box::Interface::HEIGHT * 4, "128x128 Display", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW");
    }

    te::Engine engine{"Tracktion Hello World"};
    te::Edit edit{engine, te::createEmptyEdit(engine), te::Edit::forEditing, nullptr, 0};

    edit.ensureNumberOfAudioTracks(1);
    auto first_track = te::getAudioTracks(edit)[0];

    box::Interface interface{window};
    std::unique_ptr<box::TrackManager> track_manager = std::make_unique<box::TrackManager>(*first_track);

    te::Plugin * four_osc = edit.getPluginCache().createNewPlugin(te::FourOscPlugin::xmlTypeName, {}).get();
    track_manager->AddPlugin(std::make_unique<box::FourOscManager>(four_osc));
    track_manager->SetActivePlugin(0);
    te::Plugin * delay = edit.getPluginCache().createNewPlugin(te::DelayPlugin::xmlTypeName, {}).get();
    track_manager->AddPlugin(std::make_unique<box::Delay>(delay));

    box::Manager manager;
    manager.AddTrackManager(0, std::move(track_manager));

    box::Mesh m{
        {
            // Positions      // Texture Coords
            -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,   // Top-left
            -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // Bottom-left
             0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // Bottom-right
             0.5f,  0.5f, 0.0f,  1.0f, 1.0f    // Top-right
        },
        {
            0, 1, 2,  // First triangle (Top-left, Bottom-left, Bottom-right)
            2, 3, 0   // Second triangle (Bottom-right, Top-right, Top-left)
        }
    };

    box::Shader pixel_shader{"shader/texture.vert", "shader/texture.frag"};

    try {
        auto &transport = edit.getTransport();
        transport.play(false);
        while (!interface.ShouldClose()) {
            // poll and handle events
            box::Event event;
            while (interface.PollEvent(event)) {
                manager.HandleEvent(event);
            }

            interface.PrepRender();

            // do rendering
            {
                // manager.Render(interface);
                m.Render(pixel_shader, box::Mesh::RenderMode::SolidColor);
            }

            interface.Display();
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
