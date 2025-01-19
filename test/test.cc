#include "core/app.hh"
#include "core/interface.hh"
#include "core/util.hh"
#include "raylib.h"
#include <chrono>
#include <filesystem>
#include <gtest/gtest.h>
#include <iostream>
#include <random>

#include "test_util.hh"

TEST(ExampleTest, BasicAssertions)
{
    EXPECT_EQ(1 + 1, 2);
    EXPECT_TRUE(true);
}

TEST(VirtualMidi, Environment)
{

    te::Engine engine{"Tracktion Hello World"};
    engine.getDeviceManager().createVirtualMidiDevice("box_midi");
    engine.getDeviceManager().setDefaultMidiInDevice("box_midi");
    auto ptr = engine.getDeviceManager().getDefaultMidiInDevice();
    EXPECT_NE(ptr, nullptr);
}

TEST(Fuzzer, DontCrash)
{
    bool completed_successfully = false;

    try
    {
        // Force stdout to flush immediately after each write
        std::cout.sync_with_stdio(true);
        setvbuf(stdout, nullptr, _IONBF, 0);

        std::cout << "Starting fuzzer test..." << std::endl;

        const juce::ScopedJuceInitialiser_GUI initialiser;
        SetTargetFPS(60);
        te::Engine engine{"Tracktion Hello World"};
        std::filesystem::path curr_path = std::filesystem::current_path();
        juce::File my_file{juce::String{curr_path.string() + "/tmp.box"}};
        std::unique_ptr<te::Edit> my_edit = createEmptyEdit(engine, my_file);
        te::Edit &edit = *my_edit;
        edit.ensureNumberOfAudioTracks(8);
        edit.getTransport().ensureContextAllocated();
        box::Interface interface {
        };
        box::App app(engine, edit);
        box::APP = &app;

        // Set up random number generation with a fixed seed for reproducibility
        const unsigned int seed = std::random_device{}();
        std::cout << "Random seed: " << seed << std::endl;
        std::mt19937 gen(seed);

        std::vector<KeyboardKey> available_keys;
        for (const auto &[key, _] : interface.keys_)
        {
            available_keys.push_back(key);
        }

        std::uniform_int_distribution<> key_dist(0, available_keys.size() - 1);
        std::uniform_int_distribution<> event_dist(0, 1);

        auto start_time = std::chrono::steady_clock::now();
        const auto duration = std::chrono::seconds(30);

        // arm audio devices
        {
            auto &dm = engine.getDeviceManager();
            for (int i = 0; i < dm.getNumWaveInDevices(); i++)
            {
                if (auto wip = dm.getWaveInDevice(i))
                {
                    wip->setStereoPair(false);
                    wip->setEnabled(true);
                }
                else if (auto mip = dm.getMidiInDevice(i))
                {
                    mip->setEnabled(true);
                }
            }
        }

        unsigned long event_count = 0;

        while (!interface.ShouldClose())
        {
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);

            if (elapsed >= duration)
            {
                break;
            }

            box::Event event;
            KeyboardKey random_key = available_keys[key_dist(gen)];
            bool is_press = event_dist(gen) == 0;

            event.type = is_press ? box::EventType::KeyPress : box::EventType::KeyRelease;
            event.value = static_cast<int>(random_key);

            // Log the event before processing it
            std::cout << "Event " << event_count++ << " at " << elapsed.count()
                      << "ms: " << (is_press ? "PRESS" : "RELEASE") << " " << KeyToString(random_key) << std::endl;

            app.HandleEvent(event);

            interface.PreRender();
            {
                app.Render(interface);
            }
            interface.PostRender();
        }

        std::cout << "Fuzzer completed successfully after " << event_count << " events" << std::endl;
        completed_successfully = true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        throw;
    }

    EXPECT_TRUE(completed_successfully);
}
