#include <filesystem>
#include <iostream>

#include "core/app.hh"
#include "core/interface.hh"
#include "core/util.hh"
#include "raylib.h"
#include <gtest/gtest.h>

TEST(ExampleTest, BasicAssertions)
{
    EXPECT_EQ(1 + 1, 2);
    EXPECT_TRUE(true);
}

TEST(ALSA, PrintMidiIn)
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
    box::Interface interface {
    };

    box::App app(engine, edit);
    box::APP = &app;

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
    box::Interface interface {
    };

    box::App app(engine, edit);
    box::APP = &app;

    // test ALSA::VirtualMidi
    {
        engine.getDeviceManager().createVirtualMidiDevice("box_midi");
        engine.getDeviceManager().setDefaultMidiInDevice("box_midi");
        auto ptr = engine.getDeviceManager().getDefaultMidiInDevice();
        ASSERT_NE(ptr, nullptr);
    }
}

int main()
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
    box::Interface interface {
    };

    box::App app(engine, edit);
    box::APP = &app;

    using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono; // nanoseconds, system_clock, seconds

    sleep_for(seconds(5));


    // test ALSA::PrintMidiIn
    {
        auto vec = engine.getDeviceManager().getMidiInDevices();
        std::cout << "midiInputs.size(): " << vec.size() << std::endl;
        for (auto &x : vec)
        {
            std::cout << x->getName() << std::endl;
        }
    }

    // test ALSA::VirtualMidi
    {
        engine.getDeviceManager().createVirtualMidiDevice("box_midi");
        engine.getDeviceManager().setDefaultMidiInDevice("box_midi");
        auto ptr = engine.getDeviceManager().getDefaultMidiInDevice();
        assert(ptr != nullptr);
    }

    // // test ALSA::TransportPlay
    // {
    //     auto &transport = edit.getTransport();
    //     transport.play(false);
    //     assert(transport.isPlaying() == true);
    //     using namespace std::this_thread; // sleep_for, sleep_until
    //     using namespace std::chrono; // nanoseconds, system_clock, seconds

    //     sleep_for(seconds(1));

    //     while (true) {
    //         double curr_pos = (transport.getPosition().inSeconds());
    //         std::cout << "curr_pos is: " << curr_pos << std::endl;
    //     }

    //     // const double tolerance = 1e-5f;
    //     // std::cout << "curr_pos is: " << curr_pos << std::endl;
    //     // assert(std::fabs(curr_pos) > tolerance);
    // }

    return 0;
}
