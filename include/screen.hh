#pragma once
#include "interface.hh"

namespace box {

class Screen {
public:
    virtual void render(Interface& interface) = 0;
    virtual void handle_event(const Event& event) = 0;
    virtual ~Screen() {}
};

} // namespace box
