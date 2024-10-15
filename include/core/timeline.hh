#pragma once

#include "util.hh"
#include "interface.hh"

namespace box {

struct Timeline {

    enum class ScreenState {
        Overview,
    };

    void Render(Interface& interface);
    void HandleEvent(const Event& event);

    ScreenState screen_state_ = ScreenState::Overview;

};

} // namespace box

