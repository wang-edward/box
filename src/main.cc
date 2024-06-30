#include "util.hh"
#include "interface.hh"
#include "screen_manager.hh"

#include <random>

int main() {
    Interface interface;

    // Initialize the screen manager
    ScreenManager screenManager;
    // 
    // screenManager.addScreen("trackEditor", std::make_unique<TrackEditorScreen>());
    // screenManager.addScreen("mixer", std::make_unique<MixerScreen>());
    // screenManager.addScreen("pianoRoll", std::make_unique<PianoRollScreen>());
    // 
    // screenManager.setActiveScreen("trackEditor");

    std::random_device rd;  
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_xy(1, 128);
    std::uniform_int_distribution<> dis_color(1, 255);

    while (!interface.shouldClose()) {
        // // Poll and handle events
        Event event;
        while (interface.pollEvent(event)) {
            screenManager.handleEvent(event);
        }

        int x = dis_xy(gen);
        int y = dis_xy(gen);
        uint8_t r = dis_color(gen);
        uint8_t g = dis_color(gen);
        uint8_t b = dis_color(gen);

        Color random_color = Color{r, g, b};
        interface.drawPixel(x, y, random_color);


        // Render to the inactive buffer
        screenManager.render(interface);

        // Swap buffers
        interface.swapBuffers();

        // Draw the active buffer to the screen
        interface.drawToScreen();
    }

    return 0;
}
