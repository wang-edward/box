#pragma once
#include "screen.hh"
#include <memory>
#include <map>

class ScreenManager {
    std::map<ScreenType, std::unique_ptr<Screen>> screens;
    ScreenType activeScreen;

public:
    ScreenManager();
    void addScreen(ScreenType type, std::unique_ptr<Screen> screen);
    void setActiveScreen(ScreenType type);
    void render();
    void handleEvent(const Event &event);
};
