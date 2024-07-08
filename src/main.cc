#include "util.hh"
#include "interface.hh"
#include "track_manager.hh"
#include "manager.hh"
#include "screen_demo_1stripe.hh"
#include "screen_demo_2bomb.hh"
#include "screen_four_osc.hh"

#include <random>
#include <iostream>

void print(std::string s) {
    std::cout << s << std::endl;
}

int main() {

    te::Engine engine{"Tracktion Hello World"};
    te::Edit edit{engine, te::createEmptyEdit(engine), te::Edit::forEditing, nullptr, 0};

    edit.ensureNumberOfAudioTracks(1);
    auto first_track = te::getAudioTracks(edit)[0];

    box::Interface interface;
    std::unique_ptr<box::TrackManager> trackManager = std::make_unique<box::TrackManager>(*first_track);

    trackManager->addScreen(box::ScreenType::GraphicsDemo1Stripe, std::make_unique<box::GraphicsDemo1Stripe>());
    trackManager->addScreen(box::ScreenType::GraphicsDemo2Bomb, std::make_unique<box::GraphicsDemo2Bomb>());
    trackManager->setActiveScreen(box::ScreenType::GraphicsDemo1Stripe);

    box::Manager manager;
    manager.addTrackManager(0, std::move(trackManager));

    try {
        while (!interface.shouldClose()) {
            // poll and handle events
            box::Event event;
            while (interface.pollEvent(event)) {
                manager.handleEvent(event);
            }

            // render
            manager.render(interface);

            interface.swapBuffers();
            interface.drawToScreen();
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
