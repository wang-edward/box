#include "util.hh"
#include "interface.hh"
#include "screen_manager.hh"
#include "screen_demo_1stripe.hh"
#include "screen_four_osc.hh"

#include <random>
#include <iostream>

void print(std::string s) {
    std::cout << s << std::endl;
}

int main() {
    Interface interface;
    ScreenManager screenManager;

    te::Engine engine{"Tracktion Hello World"};
    te::Edit edit{engine, te::createEmptyEdit(engine), te::Edit::forEditing, nullptr, 0};
    
    screenManager.addScreen("demo", std::make_unique<GraphicsDemoScreen1>());
    screenManager.setActiveScreen("demo");

    while (!interface.shouldClose()) {
        // Poll and handle events
        Event event;
        while (interface.pollEvent(event)) {
            screenManager.handleEvent(edit, event);
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
