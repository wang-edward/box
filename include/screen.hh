#pragma once
#include "interface.hh"

namespace box {

class Screen {
public:
    virtual void render(Interface& interface) = 0;
    virtual void handleEvent(te::Edit& edit, const Event& event) = 0;
    virtual ~Screen() {}
};

} // namespace box
