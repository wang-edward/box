#pragma once
#include "util.hh"

class Screen {
public:
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void handleEvent(const Event &event) = 0;
    virtual ~Screen() {}
};
