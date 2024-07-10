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
    std::unique_ptr<box::TrackManager> track_manager = std::make_unique<box::TrackManager>(*first_track);

    track_manager->add_screen(box::ScreenType::GraphicsDemo1Stripe, std::make_unique<box::GraphicsDemo1Stripe>());
    track_manager->add_screen(box::ScreenType::GraphicsDemo2Bomb, std::make_unique<box::GraphicsDemo2Bomb>());
    track_manager->set_active_screen(box::ScreenType::GraphicsDemo1Stripe);

    box::Manager manager;
    manager.add_track_manager(0, std::move(track_manager));

    try {
        while (!interface.should_close()) {
            // poll and handle events
            box::Event event;
            while (interface.poll_event(event)) {
                manager.handle_event(event);
            }

            // render
            manager.render(interface);

            interface.swap_buffers();
            interface.draw_to_screen();
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
