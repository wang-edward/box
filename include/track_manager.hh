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

    void add_screen(ScreenType screenType, std::unique_ptr<Screen> screen);
    void set_active_screen(ScreenType screenType);
    ScreenType get_active_screen();

    void handle_event(const Event& event);
    void render(Interface& Interface);

private:
    std::map<ScreenType, std::unique_ptr<Screen>> screens_;
    ScreenType active_screen_;
    te::AudioTrack &track_;
};

} // namespace box
