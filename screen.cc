#include "io_interface.hh"
#include "color.hh"
int main() {
    IOInterface ioInterface;

    // Initialize the screen manager
    // ScreenManager screenManager;
    // 
    // screenManager.addScreen("trackEditor", std::make_unique<TrackEditorScreen>());
    // screenManager.addScreen("mixer", std::make_unique<MixerScreen>());
    // screenManager.addScreen("pianoRoll", std::make_unique<PianoRollScreen>());
    // 
    // screenManager.setActiveScreen("trackEditor");

    while (!ioInterface.shouldClose()) {
        // // Poll and handle events
        // Event event;
        // while (ioInterface.pollEvent(event)) {
        //     screenManager.handleEvent(event);
        // }

        // // Render to the inactive buffer
        // screenManager.render(ioInterface);

        // // Swap buffers
        // ioInterface.swapBuffers();

        // // Draw the active buffer to the screen
        // ioInterface.drawToScreen();
    }

    return 0;
}
