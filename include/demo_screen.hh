#include "interface.hh"
#include "screen.hh"
#include <iostream>
#include <chrono>

class DemoScreen : public Screen {
public:
    DemoScreen() : lastUpdateTime(std::chrono::steady_clock::now()), frameCount(0) {}

    void render(Interface& interface) override {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime).count();

        if (elapsed > 10) {  // Update animation every 100 ms
            lastUpdateTime = now;
            updateAnimation();
        }

        for (int y = 0; y < Interface::HEIGHT; ++y) {
            for (int x = 0; x < Interface::WIDTH; ++x) {
                interface.drawPixel(x, y, buffer[y * Interface::WIDTH + x]);
            }
        }

        // Framerate calculation
        frameCount++;
        auto currentTime = std::chrono::steady_clock::now();
        auto timeSinceLastSecond = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFrameTime).count();
        if (timeSinceLastSecond >= 1000) {
            float fps = frameCount / (timeSinceLastSecond / 1000.0f);
            std::cout << "\rFramerate: " << fps << " FPS" << std::flush;
            frameCount = 0;
            lastFrameTime = currentTime;
        }
    }

    void handleEvent(const Event& event) override {
        std::cout << "Handled event: ";
        if (event.type == EventType::KEYPRESS) {
            std::cout << "KeyPress, value: " << event.value << std::endl;
        } else if (event.type == EventType::KEYRELEASE) {
            std::cout << "KeyRelease, value: " << event.value << std::endl;
        }
    }

private:
    std::array<Color, Interface::WIDTH * Interface::HEIGHT> buffer;
    std::chrono::steady_clock::time_point lastUpdateTime;
    std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();
    int frameCount = 0;
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
