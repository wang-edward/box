#include <random>
#include <iostream>

#include <juce_events/juce_events.h>

#include "core/util.hh"
#include "core/interface.hh"
#include "core/track_manager.hh"
#include "core/manager.hh"
#include "plugin/four_osc.hh"
#include "plugin/delay.hh"
#include "plugin/phaser.hh"
#include "plugin/chorus.hh"
#include "plugin/compressor.hh"
#include "plugin/equaliser.hh"
#include "plugin/latency.hh"
#include "plugin/reverb.hh"
#include "plugin/tone_generator.hh"

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
    std::unique_ptr<box::Track> track_manager = std::make_unique<box::Track>(*first_track);

    te::Plugin * four_osc = edit.getPluginCache().createNewPlugin(te::FourOscPlugin::xmlTypeName, {}).get();
    track_manager->AddPlugin(std::make_unique<box::FourOsc>(four_osc));

    te::Plugin * delay = edit.getPluginCache().createNewPlugin(te::DelayPlugin::xmlTypeName, {}).get();
    track_manager->AddPlugin(std::make_unique<box::Delay>(delay));

    // te::Plugin * phaser = edit.getPluginCache().createNewPlugin(te::PhaserPlugin::xmlTypeName, {}).get();
    // track_manager->AddPlugin(std::make_unique<box::Phaser>(phaser));

    // te::Plugin * chorus = edit.getPluginCache().createNewPlugin(te::ChorusPlugin::xmlTypeName, {}).get();
    // track_manager->AddPlugin(std::make_unique<box::Chorus>(chorus));

    // te::Plugin * compressor = edit.getPluginCache().createNewPlugin(te::CompressorPlugin::xmlTypeName, {}).get();
    // track_manager->AddPlugin(std::make_unique<box::Compressor>(compressor));

    // te::Plugin * equaliser = edit.getPluginCache().createNewPlugin(te::EqualiserPlugin::xmlTypeName, {}).get();
    // track_manager->AddPlugin(std::make_unique<box::Equaliser>(equaliser));

    // te::Plugin * latency = edit.getPluginCache().createNewPlugin(te::LatencyPlugin::xmlTypeName, {}).get();
    // track_manager->AddPlugin(std::make_unique<box::Latency>(latency));

    // te::Plugin * reverb = edit.getPluginCache().createNewPlugin(te::ReverbPlugin::xmlTypeName, {}).get();
    // track_manager->AddPlugin(std::make_unique<box::Reverb>(reverb));

    // te::Plugin * tone_generator = edit.getPluginCache().createNewPlugin(te::ToneGeneratorPlugin::xmlTypeName, {}).get();
    // track_manager->AddPlugin(std::make_unique<box::ToneGenerator>(tone_generator));

    track_manager->SetActivePlugin(1);

    box::Manager manager;
    manager.AddTrack(0, std::move(track_manager));

    // box::Mesh m{
    //     {
    //         // Positions      // Texture Coords
    //         -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,   // Top-left
    //         -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // Bottom-left
    //          0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // Bottom-right
    //          0.5f,  0.5f, 0.0f,  1.0f, 1.0f    // Top-right
    //     },
    //     {
    //         0, 1, 2,  // First triangle (Top-left, Bottom-left, Bottom-right)
    //         2, 3, 0   // Second triangle (Bottom-right, Top-right, Top-left)
    //     }
    // };

    box::Shader pixel_shader{"shader/texture.vert", "shader/texture.frag"};
    box::Texture red_tex;
    red_tex.LoadFromFile("assets/red_texture.png");

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
                // m.Render(pixel_shader, &red_tex);
                manager.Render(interface);
            }

            interface.Display();
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
