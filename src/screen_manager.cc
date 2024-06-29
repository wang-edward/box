#include "screen_manager.hh"

ScreenManager:: ScreenManager(): activeScreen{ScreenType::MAIN} {}

void ScreenManager:: addScreen(ScreenType type, std::unique_ptr<Screen> screen) {
    screens[type] = std::move(screen);
}

void ScreenManager:: render() {
    screens[activeScreen]->render();
}

void ScreenManager:: handleEvent(const Event &event) {
    screens[activeScreen]->handleEvent(event);
}
