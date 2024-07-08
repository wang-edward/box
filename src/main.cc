#include "util.hh"
#include "interface.hh"
#include "track_manager.hh"
#include "screen_demo_1stripe.hh"
#include "screen_demo_2bomb.hh"
#include "screen_four_osc.hh"

#include <random>
#include <iostream>

void print(std::string s) {
    std::cout << s << std::endl;
}

int main() {
    Interface interface;
    TrackManager TrackManager;

    te::Engine engine{"Tracktion Hello World"};
    te::Edit edit{engine, te::createEmptyEdit(engine), te::Edit::forEditing, nullptr, 0};
    
    TrackManager.addScreen(ScreenType::GraphicsDemo1Stripe, std::make_unique<GraphicsDemo1Stripe>());
    TrackManager.addScreen(ScreenType::GraphicsDemo2Bomb, std::make_unique<GraphicsDemo2Bomb>());
    TrackManager.setActiveScreen(ScreenType::GraphicsDemo1Stripe);

    while (!interface.shouldClose()) {
        // Poll and handle events
        Event event;
        while (interface.pollEvent(event)) {
            TrackManager.handleEvent(edit, event);
            if (event.type == EventType::KeyPress && event.value == GLFW_KEY_SPACE) {
                ScreenType curr = TrackManager.getActiveScreen();
                if (curr == ScreenType::GraphicsDemo1Stripe) {
                    TrackManager.setActiveScreen(ScreenType::GraphicsDemo2Bomb);
                } else if (curr == ScreenType::GraphicsDemo2Bomb) {
                    TrackManager.setActiveScreen(ScreenType::GraphicsDemo1Stripe);
                }
            }
        }

        // Render to the inactive buffer
        TrackManager.render(interface);

        // Swap buffers
        interface.swapBuffers();

        // Draw the active buffer to the screen
        interface.drawToScreen();
    }

    return 0;
}
