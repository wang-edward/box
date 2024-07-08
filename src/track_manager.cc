#include "track_manager.hh"

#include "util.hh"

namespace box {

void assert_screens(const std::map<ScreenType, std::unique_ptr<Screen>> &screens, ScreenType screenType, std::string name) {
    if (screens.find(screenType) == screens.end()) {
        throw std::runtime_error{"TrackManager::" + name + "(): enum not found [" + to_string(screenType) + "]"};
    }
}

TrackManager:: TrackManager(te::AudioTrack &track): track{track} {

}

void TrackManager:: addScreen(ScreenType screenType, std::unique_ptr<Screen> screen) {
    screens[screenType] = std::move(screen);
}

void TrackManager:: setActiveScreen(ScreenType screenType) {
    activeScreen = screenType;
}

ScreenType TrackManager:: getActiveScreen() {
    return activeScreen;
}

void TrackManager:: handleEvent(const Event& event) {
    assert_screens(screens, activeScreen, "handleEvent");

    if (event.type == box::EventType::KeyPress && event.value == GLFW_KEY_SPACE) {
        if (activeScreen == ScreenType::GraphicsDemo1Stripe) {
            setActiveScreen(ScreenType::GraphicsDemo2Bomb);
        } else if (activeScreen == ScreenType::GraphicsDemo2Bomb) {
            setActiveScreen(ScreenType::GraphicsDemo1Stripe);
        }

        return;
    }

    screens[activeScreen]->handleEvent(event);
}

void TrackManager:: render(Interface& interface) {
    assert_screens(screens, activeScreen, "render");
    screens[activeScreen]->render(interface);
}

} // namespace box
