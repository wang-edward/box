#pragma once
#include "util.hh"
#include "interface.hh"
#include "screen.hh"
#include <map>
#include <memory>
#include <string>

class ScreenManager {
public:
    void addScreen(ScreenType screenType, std::unique_ptr<Screen> screen);

    void setActiveScreen(ScreenType screenType);
    ScreenType getActiveScreen();

    void handleEvent(te::Edit &edit, const Event& event);

    void render(Interface& Interface);

private:
    std::map<ScreenType, std::unique_ptr<Screen>> screens;
    ScreenType activeScreen;
};
