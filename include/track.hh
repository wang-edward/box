#pragma once
#include "util.hh"
#include "interface.hh"
#include "screen.hh"
#include <map>
#include <memory>
#include <string>

namespace box {

class Track {
public:
    Track(te::AudioTrack &track);

    void AddScreen(ScreenType screenType, std::unique_ptr<Screen> screen);
    void SetActiveScreen(ScreenType screenType);
    ScreenType GetActiveScreen();

    void HandleEvent(const Event& event);
    void Render(Interface& Interface);

private:
    std::map<ScreenType, std::unique_ptr<Screen>> screens_;
    ScreenType active_screen_;
    te::AudioTrack &track_;
};

} // namespace box
