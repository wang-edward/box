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
    const juce::ScopedJuceInitialiser_GUI initialiser; // TODO what does this do

    te::Engine engine{"Tracktion Hello World"};
    te::Edit edit{engine, te::createEmptyEdit(engine), te::Edit::forEditing, nullptr, 0};
    // box::EDIT = &edit;

    edit.ensureNumberOfAudioTracks(8);
    auto first_track = te::getAudioTracks(edit)[0];

    box::Interface interface{};
    std::unique_ptr<box::Track> track_manager = std::make_unique<box::Track>(*first_track);

    te::Plugin * four_osc = edit.getPluginCache().createNewPlugin(te::FourOscPlugin::xmlTypeName, {}).get();
    track_manager->AddPlugin(std::make_unique<box::FourOsc>(four_osc));

    te::Plugin * delay = edit.getPluginCache().createNewPlugin(te::DelayPlugin::xmlTypeName, {}).get();
    track_manager->AddPlugin(std::make_unique<box::Delay>(delay));

    // te::Plugin * phaser = edit.getPluginCache().createNewPlugin(te::PhaserPlugin::xmlTypeName, {}).get();
    // track_manager->AddPlugin(std::make_unique<box::Phaser>(phaser));

    // te::Plugin * chorus = edit.getPluginCache().createNewPlugin(te::ChorusPlugin::xmlTypeName, {}).get();
    // track_manager->AddPlugin(std::make_unique<box::Chorus>(chorus));

    // te::Plugin * compressor = edit.getPluginCache().createNewPlugin(te::CompressorPlugin::xmlTypeName, {}).get();
    // track_manager->AddPlugin(std::make_unique<box::Compressor>(compressor));

    // te::Plugin * equaliser = edit.getPluginCache().createNewPlugin(te::EqualiserPlugin::xmlTypeName, {}).get();
    // track_manager->AddPlugin(std::make_unique<box::Equaliser>(equaliser));

    // te::Plugin * latency = edit.getPluginCache().createNewPlugin(te::LatencyPlugin::xmlTypeName, {}).get();
    // track_manager->AddPlugin(std::make_unique<box::Latency>(latency));

    // te::Plugin * reverb = edit.getPluginCache().createNewPlugin(te::ReverbPlugin::xmlTypeName, {}).get();
    // track_manager->AddPlugin(std::make_unique<box::Reverb>(reverb));

    // te::Plugin * tone_generator = edit.getPluginCache().createNewPlugin(te::ToneGeneratorPlugin::xmlTypeName, {}).get();
    // track_manager->AddPlugin(std::make_unique<box::ToneGenerator>(tone_generator));

    track_manager->SetActivePlugin(1);

    box::Manager manager(te::getAudioTracks(edit));
    // manager.AddTrack(std::move(track_manager));

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
