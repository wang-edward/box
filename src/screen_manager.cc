#include "screen_manager.hh"

void ScreenManager:: addScreen(const std::string& name, std::unique_ptr<Screen> screen) {
    screens[name] = std::move(screen);
}

void ScreenManager:: setActiveScreen(const std::string& name) {
    activeScreen = screens[name].get();
}

void ScreenManager:: handleEvent(te::Edit &edit, const Event& event) {
    if (activeScreen) {
        activeScreen->handleEvent(edit, event);
    }
}

void ScreenManager:: render(Interface& Interface) {
    if (activeScreen) {
        activeScreen->render(Interface);
    }
}
