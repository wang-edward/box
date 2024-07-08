#pragma once
#include "util.hh"
#include "interface.hh"
#include "screen.hh"
#include <map>
#include <memory>
#include <string>

namespace box {

class TrackManager {
public:
    TrackManager(te::AudioTrack &track);

    void addScreen(ScreenType screenType, std::unique_ptr<Screen> screen);
    void setActiveScreen(ScreenType screenType);
    ScreenType getActiveScreen();

    void handleEvent(const Event& event);
    void render(Interface& Interface);

private:
    std::map<ScreenType, std::unique_ptr<Screen>> screens;
    ScreenType activeScreen;
    te::AudioTrack &track;
};

} // namespace box
