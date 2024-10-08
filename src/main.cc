#include <random>
#include <iostream>

#include <juce_events/juce_events.h>

#include "core/util.hh"
#include "core/interface.hh"
#include "core/track.hh"
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

#include "raylib.h"

int main() {
   const juce::ScopedJuceInitialiser_GUI initialiser; // you need this otherwise the entire engine doesn't work??

    te::Engine engine{"Tracktion Hello World"};
    te::Edit edit{engine, te::createEmptyEdit(engine), te::Edit::forEditing, nullptr, 0};
    box::EDIT = &edit;
    edit.ensureNumberOfAudioTracks(8);
    box::Interface interface{};
    box::Manager manager(edit);

    try {
        auto &transport = edit.getTransport();
        transport.play(false);
        while (!interface.ShouldClose()) {
            // poll and handle events
            box::Event event;
            if (interface.PollEvent(event)) {
                manager.HandleEvent(event);
            }

            interface.PreRender();

            // do rendering
            {
                manager.Render(interface);
            }

            interface.PostRender();
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
