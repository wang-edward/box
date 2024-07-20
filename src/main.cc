#include "util.hh"
#include "interface.hh"
#include "track.hh"
#include "manager.hh"
#include "screen_demo_1stripe.hh"
#include "screen_demo_2bomb.hh"
#include "screen_four_osc.hh"

#include <random>
#include <iostream>

static void print(std::string s) {
    std::cout << s << std::endl;
}

int main() {

    te::Engine engine{"Tracktion Hello World"};
    te::Edit edit{engine, te::createEmptyEdit(engine), te::Edit::forEditing, nullptr, 0};

    edit.ensureNumberOfAudioTracks(1);
    auto first_track = te::getAudioTracks(edit)[0];

    box::Interface interface;
    std::unique_ptr<box::Track> track_manager = std::make_unique<box::Track>(*first_track);

    track_manager->AddScreen(box::ScreenType::GraphicsDemo1Stripe, std::make_unique<box::GraphicsDemo1Stripe>());
    track_manager->AddScreen(box::ScreenType::GraphicsDemo2Bomb, std::make_unique<box::GraphicsDemo2Bomb>());
    track_manager->SetActiveScreen(box::ScreenType::GraphicsDemo1Stripe);

    box::Manager manager;
    manager.AddTrackManager(0, std::move(track_manager));

    try {
        while (!interface.ShouldClose()) {
            // poll and handle events
            box::Event event;
            while (interface.PollEvent(event)) {
                manager.HandleEvent(event);
            }

            // render
            manager.Render(interface);

            interface.SwapBuffers();
            interface.DrawToScreen();
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
