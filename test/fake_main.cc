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
    engine.getDeviceManager().rescanMidiDeviceList();

    std::cout << "-----------" << std::endl;
    std::cout << "begin tests" << std::endl;

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

    return 0;
}
