#pragma once

#include "util.hh"
#include "interface.hh"

namespace box {

struct Timeline {

    enum class ScreenState {
        Overview,
    };

    ScreenState screen_state_ = ScreenState::Overview;
    size_t scroll_offset_ = 0;
    double radius_ = 8.0;

    void Render(Interface& interface);
    void HandleEvent(const Event& event);
    void print_timeline();
};

} // namespace box

