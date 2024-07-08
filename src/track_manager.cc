#include "track_manager.hh"
#include "util.hh"

namespace box {

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

    screens[activeScreen]->handleEvent(edit, event);
void TrackManager:: handleEvent(const Event& event) {
}

void TrackManager:: render(Interface& interface) {
    screens[activeScreen]->render(interface);
}

} // namespace box
