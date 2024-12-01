#pragma once

#include "util.hh"
#include "interface.hh"

namespace box {

struct Timeline {

    enum class ScreenState {
        Overview,
    };

    enum class PlayheadMode {
        Detached,
        Locked,
    };

    struct BeatWindow {
        float start;
        float len;
        float LeftEdge() {return start;}
        float RightEdge() {return start + len;}
    };

    struct BeatFrame {
        float center;
        float radius;
        float LeftEdge() {return center - radius;}
        float RightEdge() {return center + radius;}
        BeatFrame(float c, float &r): center{c}, radius{r} {}
    };

    struct Position {
        float secs;
        float beats;
    };

    float bar_width_ = 4.0; // Assuming a 4/4 time signature by default TODO update with time signature changes
    float step_size_ = 4.0; // Move cursor by 4 beat (1 bar)
    float radius_ = 8.0;
    BeatWindow cursor_ = {0.0, bar_width_};
    BeatFrame frame_ = {0.0, radius_};

    ScreenState screen_state_ = ScreenState::Overview;
    PlayheadMode playhead_mode_  = PlayheadMode::Detached;
    size_t scroll_offset_ = 0;

    Timeline();

    void Render(Interface& interface);
    void HandleEvent(const Event& event);
    void print_timeline();
};

} // namespace box

