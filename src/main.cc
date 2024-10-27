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
    const juce::ScopedJuceInitialiser_GUI initialiser; // you need this otherwise the entire engine doesn't work??

    te::Engine engine{"Tracktion Hello World"};
    te::Edit edit{engine, te::createEmptyEdit(engine), te::Edit::forEditing, nullptr, 0};
    edit.ensureNumberOfAudioTracks(8);
    box::Interface interface{};

    box::App app(edit);
    box::APP = &app;

    try
    {

        {
            // for (auto& midiIn : engine.getDeviceManager().getMidiInDevices())
            // {
            //     midiIn->setMonitorMode (te::InputDevice::MonitorMode::automatic);
            //     midiIn->setEnabled (true);
            // }

            edit.getTransport().ensureContextAllocated();

            int index = 0;

            for (auto instance : edit.getAllInputDevices())
            {
                if (instance->getInputDevice().getDeviceType() == te::InputDevice::physicalMidiDevice)
                {
                    auto t = box::APP->base_tracks_[index];
                    {
                        [[ maybe_unused ]] auto res = instance->setTarget (t->itemID, true, &edit.getUndoManager(), 0);
                        instance->setRecordingEnabled(*t, true);

                        index++;
                    }
                }
            }

            // edit.restartPlayback();
        }
        auto &transport = edit.getTransport();
        // transport.ensureContextAllocated(true);
        // transport.play(false);
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
