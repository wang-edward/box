#pragma once
#include "interface.hh"

namespace box {

class Screen {
public:
    virtual void Render(Interface& interface) = 0;
    virtual void HandleEvent(const Event& event) = 0;
    virtual ~Screen() {}
};

} // namespace box
