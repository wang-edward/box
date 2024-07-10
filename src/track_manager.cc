#include "track_manager.hh"

#include "util.hh"

namespace box {

void assert_screens(const std::map<ScreenType, std::unique_ptr<Screen>> &screens, ScreenType screen_type, std::string name) {
    if (screens.find(screen_type) == screens.end()) {
        throw std::runtime_error{"TrackManager::" + name + "(): enum not found [" + to_string(screen_type) + "]"};
    }
}

TrackManager:: TrackManager(te::AudioTrack &track): track_{track} {

}

void TrackManager:: add_screen(ScreenType screenType, std::unique_ptr<Screen> screen) {
    screens_[screenType] = std::move(screen);
}

void TrackManager:: set_active_screen(ScreenType screenType) {
    active_screen_ = screenType;
}

ScreenType TrackManager:: get_active_screen() {
    return active_screen_;
}

void TrackManager:: handle_event(const Event& event) {
    assert_screens(screens_, active_screen_, "handleEvent");

    if (event.type_ == box::EventType::KeyPress && event.value_ == GLFW_KEY_SPACE) {
        if (active_screen_ == ScreenType::GraphicsDemo1Stripe) {
            set_active_screen(ScreenType::GraphicsDemo2Bomb);
        } else if (active_screen_ == ScreenType::GraphicsDemo2Bomb) {
            set_active_screen(ScreenType::GraphicsDemo1Stripe);
        }

        return;
    }

    screens_[active_screen_]->handle_event(event);
}

void TrackManager:: render(Interface& interface) {
    assert_screens(screens_, active_screen_, "render");
    screens_[active_screen_]->render(interface);
}

} // namespace box
