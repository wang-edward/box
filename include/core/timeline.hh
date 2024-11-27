#pragma once

#include "util.hh"
#include "interface.hh"

namespace box {

struct Timeline {

    enum class ScreenState {
        Overview,
    };

    struct BeatRange {
        double left_edge;
        double right_edge;
    };

    struct Position {
        double secs;
        double beats;
    };

    BeatRange cursor_;
    double bar_width_ = 4.0; // Assuming a 4/4 time signature by default TODO update with time signature changes
    double step_size_ = 4.0; // Move cursor by 4 beat (1 bar)
    double radius_ = 8.0;
    ScreenState screen_state_ = ScreenState::Overview;
    size_t scroll_offset_ = 0;

    Timeline();

    void Render(Interface& interface);
    void HandleEvent(const Event& event);
    void print_timeline();
};

} // namespace box

