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

TEST(ALSA, PrintMidiIn)
{
    const juce::ScopedJuceInitialiser_GUI initialiser; // need this

    te::Engine engine{"Tracktion Hello World"};
    std::filesystem::path curr_path = std::filesystem::current_path();
    juce::File my_file{juce::String{curr_path.string() + "/tmp.box"}};

    std::unique_ptr<te::Edit> my_edit = createEmptyEdit(engine, my_file);
    te::Edit &edit = *my_edit;
    edit.ensureNumberOfAudioTracks(8);
    edit.getTransport().ensureContextAllocated();
    // box::Interface interface {
    // };

    // box::App app(engine, edit);
    // box::APP = &app;

    // test ALSA::PrintMidiIn
    {
        auto vec = engine.getDeviceManager().getMidiInDevices();
        std::cout << "midiInputs.size(): " << vec.size() << std::endl;
        for (auto &x : vec)
        {
            std::cout << x->getName() << std::endl;
        }
    }
}

// this generates different code than main()
// where the assert(ptr != nullptr) passes in main(),
//   ASSERT_NE(ptr, nullptr) fails.
TEST(ALSA, VirtualMidi)
{
    const juce::ScopedJuceInitialiser_GUI initialiser; // need this
    SetTargetFPS(60);

    te::Engine engine{"Tracktion Hello World"};
    std::filesystem::path curr_path = std::filesystem::current_path();
    juce::File my_file{juce::String{curr_path.string() + "/tmp.box"}};

    std::unique_ptr<te::Edit> my_edit = createEmptyEdit(engine, my_file);
    te::Edit &edit = *my_edit;
    edit.ensureNumberOfAudioTracks(8);
    edit.getTransport().ensureContextAllocated();
    // box::Interface interface {
    // };

    // box::App app(engine, edit);
    // box::APP = &app;

    // test ALSA::VirtualMidi
    {
        engine.getDeviceManager().createVirtualMidiDevice("box_midi");
        engine.getDeviceManager().setDefaultMidiInDevice("box_midi");
        auto ptr = engine.getDeviceManager().getDefaultMidiInDevice();
        ASSERT_NE(ptr, nullptr);
    }
}
