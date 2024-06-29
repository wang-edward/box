#pragma once
#include <array>
#include "util.hh"

class IOInterface {
    constexpr static DeviceType DisplayType = DeviceType::EMULATOR;
    constexpr static DeviceType InputType = DeviceType::EMULATOR;
    constexpr static int WIDTH = 128;
    constexpr static int HEIGHT = 128;
 
    std::array<Color, WIDTH * HEIGHT> activeBuffer;
    std::array<Color, WIDTH * HEIGHT> inactiveBuffer;
 
public:
    IOInterface();
    ~IOInterface();
    void drawPixel(int x, int y, Color color);
    void swapBuffers();
    void drawToScreen() const;
    bool pollEvent(Event &event);
};
