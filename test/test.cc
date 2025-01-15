#include <gtest/gtest.h>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <random>
#include "core/util.hh"
#include "core/interface.hh"
#include "core/app.hh"
#include "raylib.h"

TEST(ExampleTest, BasicAssertions)
{
    EXPECT_EQ(1 + 1, 2);
    EXPECT_TRUE(true);
}

TEST(Fuzzer, DontCrash)
{
    const juce::ScopedJuceInitialiser_GUI initialiser;
    SetTargetFPS(60);
    te::Engine engine{"Tracktion Hello World"};
    std::filesystem::path curr_path = std::filesystem::current_path();
    juce::File my_file {juce::String{curr_path.string() + "/tmp.box"}};
    std::unique_ptr<te::Edit> my_edit = createEmptyEdit(engine, my_file);
    te::Edit &edit = *my_edit;
    edit.ensureNumberOfAudioTracks(8);
    edit.getTransport().ensureContextAllocated();
    box::Interface interface{};
    box::App app(engine, edit);
    box::APP = &app;

    // Set up random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Get all keys from the keys_ map into a vector for easy random access
    std::vector<KeyboardKey> available_keys;
    for (const auto& [key, _] : interface.keys_) {
        available_keys.push_back(key);
    }
    
    // Distribution for selecting random keys
    std::uniform_int_distribution<> key_dist(0, available_keys.size() - 1);
    
    // Distribution for selecting event type (press=0 or release=1)
    std::uniform_int_distribution<> event_dist(0, 1);

    // Start time
    auto start_time = std::chrono::steady_clock::now();
    const auto duration = std::chrono::seconds(30);

    // arm audio devices
    {
        auto& dm = engine.getDeviceManager();
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

    // event loop
    while (!interface.ShouldClose()) 
    {
        // Check if 30 seconds have elapsed
        auto current_time = std::chrono::steady_clock::now();
        if (current_time - start_time >= duration) {
            break;  // Exit the loop after 30 seconds
        }

        // Generate random key events
        box::Event event;
        
        // Randomly select a key
        KeyboardKey random_key = available_keys[key_dist(gen)];
        
        // Randomly decide between press and release
        bool is_press = event_dist(gen) == 0;
        
        // Create the event
        event.type = is_press ? box::EventType::KeyPress : box::EventType::KeyRelease;
        event.value = static_cast<int>(random_key);
        
        // Send the event to the application
        app.HandleEvent(event);

        interface.PreRender();
        {
            app.Render(interface);
        }
        interface.PostRender();
    }

    // If we reach here without crashing, the test passes
    EXPECT_TRUE(true);
}
