#pragma once
#include "util.hh"
#include "interface.hh"
#include "screen.hh"
#include <map>
#include <memory>
#include <string>

class ScreenManager {
public:
    void addScreen(const std::string& name, std::unique_ptr<Screen> screen);

    void setActiveScreen(const std::string& name);

    void handleEvent(const Event& event);

    void render(Interface& Interface);

private:
    std::map<std::string, std::unique_ptr<Screen>> screens;
    Screen* activeScreen;
};
