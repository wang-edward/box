#pragma once
#include "screen.hh"
#include "interface.hh"
#include <iostream>
#include <chrono>

class DemoScreen : public Screen {
public:
    DemoScreen() : lastUpdateTime(std::chrono::steady_clock::now()) {}

    void render(Interface& interface) override {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime).count();

        if (elapsed > 10) {  // Update animation every 10 ms
            lastUpdateTime = now;
            updateAnimation();
        }

        for (int y = 0; y < Interface::HEIGHT; ++y) {
            for (int x = 0; x < Interface::WIDTH; ++x) {
                interface.drawPixel(x, y, buffer[y * Interface::WIDTH + x]);
            }
        }
    }

    void handleEvent(const Event& event) override {
        std::cout << "Handled event: ";
        if (event.type == EventType::KEYPRESS) {
            std::cout << "KeyPress, key: " << event.value << std::endl;
        // } else if (event.type == EventType::MicrocontrollerEvent) {
        //     std::cout << "MicrocontrollerEvent" << std::endl;
        }
    }

private:
    std::array<Color, Interface::WIDTH * Interface::HEIGHT> buffer;
    std::chrono::steady_clock::time_point lastUpdateTime;
    int frame = 0;

    void updateAnimation() {
        frame = (frame + 1) % Interface::WIDTH;

        for (int y = 0; y < Interface::HEIGHT; ++y) {
            for (int x = 0; x < Interface::WIDTH; ++x) {
                if ((x + frame) % Interface::WIDTH < Interface::WIDTH / 2) {
                    buffer[y * Interface::WIDTH + x] = Color{255, 0, 0};  // Red
                } else {
                    buffer[y * Interface::WIDTH + x] = Color{0, 0, 255};  // Blue
                }
            }
        }
    }
};
