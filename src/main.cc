#include <random>
#include <iostream>

#include "core/util.hh"
#include "core/interface.hh"
#include "core/track_manager.hh"
#include "core/manager.hh"
#include "plugin/four_osc.hh"

int main() {
    te::Engine engine{"Tracktion Hello World"};
    te::Edit edit{engine, te::createEmptyEdit(engine), te::Edit::forEditing, nullptr, 0};

    edit.ensureNumberOfAudioTracks(1);
    auto first_track = te::getAudioTracks(edit)[0];

    box::Interface interface;
    std::unique_ptr<box::TrackManager> track_manager = std::make_unique<box::TrackManager>(*first_track);

    te::Plugin * four_osc = edit.getPluginCache().createNewPlugin(te::FourOscPlugin::xmlTypeName, {}).get();
    track_manager->AddPlugin(std::make_unique<box::FourOscManager>(four_osc));
    track_manager->SetActivePlugin(0);

    box::Manager manager;
    manager.AddTrackManager(0, std::move(track_manager));

    try {
        auto &transport = edit.getTransport();
        transport.play(false);
        while (!interface.ShouldClose()) {
            // poll and handle events
            box::Event event;
            while (interface.PollEvent(event)) {
                manager.HandleEvent(event);
            }
            interface.PrepRender();

            // do rendering
            {
                manager.Render(interface);
            }

            interface.Display();
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
