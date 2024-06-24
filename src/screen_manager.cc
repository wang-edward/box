#include "screen_manager.hh"

ScreenManager:: ScreenManager(): activeScreen_{ScreenType::MAIN} {}

void ScreenManager:: addScreen(ScreenType type, std::unique_ptr<Screen> screen) {
    screens_[type] = std::move(screen);
}

void ScreenManager:: render() {
    screens_[activeScreen_].render();
}

void ScreenManager:: handleEvent(cosnt Event &event) {
    screens_[activeScreen_].render();
}
