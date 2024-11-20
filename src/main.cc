#include <random>
#include <iostream>
#include <filesystem>

#include "core/util.hh"
#include "core/interface.hh"
#include "core/track.hh"
#include "core/app.hh"
#include "plugin/four_osc.hh"
#include "plugin/chorus.hh"

#include "raylib.h"


int main() 
{
    const juce::ScopedJuceInitialiser_GUI initialiser; // need this
    SetTargetFPS(60);

    te::Engine engine{"Tracktion Hello World"};
    // std::unique_ptr<Edit> createEmptyEdit (Engine&, const juce::File&);
    std::filesystem::path curr_path = std::filesystem::absolute(__FILE__);
    juce::File my_file {juce::String{curr_path.string()}};

    std::unique_ptr<te::Edit> my_edit = createEmptyEdit(engine, my_file);
    te::Edit &edit = *my_edit;
    edit.ensureNumberOfAudioTracks(8);
    edit.getTransport().ensureContextAllocated();
    box::Interface interface{};

    box::App app(engine, edit);
    box::APP = &app;

    try
    {
        // arm
        {
            auto& dm = engine.getDeviceManager();

            for (int i = 0; i < dm.getNumWaveInDevices(); i++)
            {
                if (auto wip = dm.getWaveInDevice(i))
                {
                    wip->setStereoPair(false);
                    // wip->setEndToEnd(true); // TODO update ?
                    wip->setEnabled(true);
                }
                else if (auto mip = dm.getMidiInDevice(i))
                {
                    // mip->setEndToEndEnabled(true); // TODO update ?
                    mip->setEnabled(true);
                }
            }
        }

        while (!interface.ShouldClose()) 
        {
            // poll and handle events
            box::Event event;
            if (interface.PollEvent(event)) 
            {
                app.HandleEvent(event);
            }

            interface.PreRender();

            // do rendering
            {
                app.Render(interface);
            }

            interface.PostRender();
        }
    } 
    catch (std::exception &e) 
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
