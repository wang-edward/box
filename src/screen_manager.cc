#include "screen_manager.hh"

void ScreenManager:: addScreen(ScreenType screenType, std::unique_ptr<Screen> screen) {
    screens[screenType] = std::move(screen);
}

void ScreenManager:: setActiveScreen(ScreenType screenType) {
    activeScreen = screenType;
}

ScreenType ScreenManager:: getActiveScreen() {
    return activeScreen;
}

void ScreenManager:: handleEvent(te::Edit &edit, const Event& event) {
    screens[activeScreen]->handleEvent(edit, event);
}

void ScreenManager:: render(Interface& interface) {
    screens[activeScreen]->render(interface);
}
