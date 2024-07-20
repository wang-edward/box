#include "track.hh"

#include "util.hh"

namespace box {

static void assert_screens(const std::map<ScreenType, std::unique_ptr<Screen>> &screens, ScreenType screen_type, std::string name) {
    if (screens.find(screen_type) == screens.end()) {
        throw std::runtime_error{"Track::" + name + "(): enum not found [" + to_string(screen_type) + "]"};
    }
}

Track:: Track(te::AudioTrack &track): track_{track} {

}

void Track:: AddScreen(ScreenType screenType, std::unique_ptr<Screen> screen) {
    screens_[screenType] = std::move(screen);
}

void Track:: SetActiveScreen(ScreenType screenType) {
    active_screen_ = screenType;
}

ScreenType Track:: GetActiveScreen() {
    return active_screen_;
}

void Track:: HandleEvent(const Event& event) {
    assert_screens(screens_, active_screen_, "handleEvent");

    if (event.type_ == box::EventType::KeyPress && event.value_ == GLFW_KEY_SPACE) {
        if (active_screen_ == ScreenType::GraphicsDemo1Stripe) {
            SetActiveScreen(ScreenType::GraphicsDemo2Bomb);
        } else if (active_screen_ == ScreenType::GraphicsDemo2Bomb) {
            SetActiveScreen(ScreenType::GraphicsDemo1Stripe);
        }

        return;
    }

    screens_[active_screen_]->HandleEvent(event);
}

void Track:: Render(Interface& interface) {
    assert_screens(screens_, active_screen_, "render");
    screens_[active_screen_]->Render(interface);
}

} // namespace box
