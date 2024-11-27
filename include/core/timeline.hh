#pragma once

#include "util.hh"
#include "interface.hh"

namespace box {

struct Timeline {

    enum class ScreenState {
        Overview,
    };

    struct BeatRange {
        float left_edge;
        float right_edge;
    };

    struct Position {
        float secs;
        float beats;
    };

    BeatRange cursor_;
    float bar_width_ = 4.0; // Assuming a 4/4 time signature by default TODO update with time signature changes
    float step_size_ = 4.0; // Move cursor by 4 beat (1 bar)
    float radius_ = 8.0;
    ScreenState screen_state_ = ScreenState::Overview;
    size_t scroll_offset_ = 0;

    Timeline();

    void Render(Interface& interface);
    void HandleEvent(const Event& event);
    void print_timeline();
};

} // namespace box

