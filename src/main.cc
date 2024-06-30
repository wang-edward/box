#include "util.hh"
#include "interface.hh"
#include "screen_manager.hh"
#include "demo_screen.hh"

#include <random>
#include <iostream>


int main() {
    Interface interface;

    // Initialize the screen manager
    ScreenManager screenManager;
    
    screenManager.addScreen("demo", std::make_unique<DemoScreen>());
    screenManager.setActiveScreen("demo");

    while (!interface.shouldClose()) {
        // Poll and handle events
        Event event;
        while (interface.pollEvent(event)) {
            screenManager.handleEvent(event);
        }

        // Render to the inactive buffer
        screenManager.render(interface);

        // Swap buffers
        interface.swapBuffers();

        // Draw the active buffer to the screen
        interface.drawToScreen();
    }

    return 0;
}
