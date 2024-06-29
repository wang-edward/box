#pragma once
#include "io_interface.hh"
#include "color.hh"
#include <map>
#include <memory>
#include <string>

class Screen {
public:
    virtual void render(IOInterface& ioInterface) = 0;
    virtual void handleEvent(const Event& event) = 0;
    virtual ~Screen() {}
};

class ScreenManager {
private:
    std::map<std::string, std::unique_ptr<Screen>> screens;
    Screen* activeScreen;

public:
    void addScreen(const std::string& name, std::unique_ptr<Screen> screen) {
        screens[name] = std::move(screen);
    }

    void setActiveScreen(const std::string& name) {
        activeScreen = screens[name].get();
    }

    void handleEvent(const Event& event) {
        if (activeScreen) {
            activeScreen->handleEvent(event);
        }
    }

    void render(IOInterface& ioInterface) {
        if (activeScreen) {
            activeScreen->render(ioInterface);
        }
    }
};
