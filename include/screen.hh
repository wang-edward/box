#pragma once
#include "interface.hh"

class Screen {
public:
    virtual void render(Interface& interface) = 0;
    virtual void handleEvent(const Event& event) = 0;
    virtual ~Screen() {}
};

