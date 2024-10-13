#pragma once

#include "util.hh"
#include "interface.hh"

namespace box {

struct Timeline {
    void Render(Interface& interface);
    void HandleEvent(const Event& event);

};

} // namespace box

