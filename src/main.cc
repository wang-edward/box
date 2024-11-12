#include <random>
#include <iostream>

#include <juce_events/juce_events.h>

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
    te::Edit edit{engine, te::createEmptyEdit(engine), te::Edit::forEditing, nullptr, 0};
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
                    wip->setEndToEnd(true);
                    wip->setEnabled(true);
                }
                else if (auto mip = dm.getMidiInDevice(i))
                {
                    mip->setEndToEndEnabled(true);
                    mip->setEnabled(true);
                }
            }

            // int track_idx = 0;
            // for (size_t i = 0; i < box::MAX_TRACKS; i++)
            // {
            //     for (auto instance : edit.getAllInputDevices())
            //     {
            //         auto device_type = instance->getInputDevice().getDeviceType();
            //         if (device_type == te::InputDevice::physicalMidiDevice ||
            //             device_type == te::InputDevice::virtualMidiDevice)
            //         {
            //             auto t = te::getAudioTracks(edit)[track_idx];
            //             if (t != nullptr)
            //             {
            //                 instance->setTargetTrack(*t, 0, true, &edit.getUndoManager());
            //                 instance->setRecordingEnabled(*t, true);
            //             }
            //         }
            //     }
            // }
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
