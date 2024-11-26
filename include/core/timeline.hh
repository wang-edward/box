#pragma once

#include "util.hh"
#include "interface.hh"

namespace box {

struct Timeline {

    enum class ScreenState {
        Overview,
    };

    te::BeatRange cursor_;
    double bar_width_ = 4.0; // Assuming a 4/4 time signature by default
    double step_size_ = 4.0; // Move cursor by 1 beat
    double radius_ = 8.0;
    ScreenState screen_state_ = ScreenState::Overview;
    size_t scroll_offset_ = 0;

    Timeline();

    void Render(Interface& interface);
    void HandleEvent(const Event& event);
    void print_timeline();
};

} // namespace box

