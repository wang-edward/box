#include <filesystem>
#include <iostream>

#include "core/app.hh"
#include "core/interface.hh"
#include "core/util.hh"
#include "raylib.h"

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

    std::cout << "-----------" << std::endl;
    std::cout << "begin tests" << std::endl;

    using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono; // nanoseconds, system_clock, seconds
    std::cout << "wait 5s for stuff to init? ..." << std::endl;
    sleep_for(seconds(5));

    // test ALSA::PrintMidiIn
    {
        std::cout << "ALSA::PrintMidiIn begin:" << std::endl;
        auto vec = engine.getDeviceManager().getMidiInDevices();
        std::cout << "midiInputs.size(): " << vec.size() << std::endl;
        for (auto &x : vec)
        {
            std::cout << "\t" << x->getName() << std::endl;
        }
        std::cout << "ALSA::PrintMidiIn end" << std::endl;
    }

    std::cout << std::endl;

    // test ALSA::VirtualMidi
    {
        std::cout << "ALSA::VirtualMidi begin" << std::endl;
        engine.getDeviceManager().createVirtualMidiDevice("box_midi");
        engine.getDeviceManager().setDefaultMidiInDevice("box_midi");
        auto ptr = engine.getDeviceManager().getDefaultMidiInDevice();
        assert(ptr != nullptr);
        std::cout << "ALSA::VirtualMidi end" << std::endl;
    }

    std::cout << "end tests" << std::endl;
    std::cout << "-----------" << std::endl;

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
