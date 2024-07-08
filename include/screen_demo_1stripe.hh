#include "interface.hh"
#include "screen.hh"
#include <iostream>
#include <chrono>
#include <array>

namespace box {

// class GraphicsDemo1Stripe : public Screen {
// public:
//     GraphicsDemo1Stripe() : lastUpdateTime(std::chrono::steady_clock::now()), useAlternateColors(false) {}

//     void render(Interface& interface) override {
//         auto now = std::chrono::steady_clock::now();
//         auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime).count();

//         if (elapsed > 100) {  // Update animation every 100 ms
//             lastUpdateTime = now;
//             updateAnimation();
//         }

//         for (int y = 0; y < Interface::HEIGHT; ++y) {
//             for (int x = 0; x < Interface::WIDTH; ++x) {
//                 interface.drawPixel(x, y, buffer[y * Interface::WIDTH + x]);
//             }
//         }
//     }

//     void handleEvent(te::Edit& edit, const Event& event) override {
//         if (event.type == EventType::KeyPress && event.value == GLFW_KEY_ENTER) {
//             useAlternateColors = !useAlternateColors;
//         }
//     }

// private:
//     std::array<Color, Interface::WIDTH * Interface::HEIGHT> buffer;
//     std::chrono::steady_clock::time_point lastUpdateTime;
//     bool useAlternateColors;

//     void updateAnimation() {
//         for (int y = 0; y < Interface::HEIGHT; ++y) {
//             for (int x = 0; x < Interface::WIDTH; ++x) {
//                 if ((x + y) % 2 == 0) {
//                     buffer[y * Interface::WIDTH + x] = useAlternateColors ? Color{0, 255, 0} : Color{255, 0, 0};  // Green or Red
//                 } else {
//                     buffer[y * Interface::WIDTH + x] = useAlternateColors ? Color{255, 255, 0} : Color{0, 0, 255};  // Yellow or Blue
//                 }
//             }
//         }
//     }
// };










// class GraphicsDemo1Stripe : public Screen {
// public:
//     GraphicsDemo1Stripe() : lastUpdateTime(std::chrono::steady_clock::now()), useAlternateColors(false) {}

//     void render(Interface& interface) override {
//         auto now = std::chrono::steady_clock::now();
//         auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime).count();

//         if (elapsed > 10) {  // Update animation every 100 ms
//             lastUpdateTime = now;
//             updateAnimation();
//         }

//         for (int y = 0; y < Interface::HEIGHT; ++y) {
//             for (int x = 0; x < Interface::WIDTH; ++x) {
//                 interface.drawPixel(x, y, buffer[y * Interface::WIDTH + x]);
//             }
//         }
//     }

//     void handleEvent(te::Edit& edit, const Event& event) override {
//         if (event.type == EventType::KeyPress && event.value == GLFW_KEY_ENTER) {
//             useAlternateColors = !useAlternateColors;
//         }
//     }

// private:
//     std::array<Color, Interface::WIDTH * Interface::HEIGHT> buffer;
//     std::chrono::steady_clock::time_point lastUpdateTime;
//     bool useAlternateColors;

//     void updateAnimation() {
//         for (int y = 0; y < Interface::HEIGHT; ++y) {
//             for (int x = 0; x < Interface::WIDTH; ++x) {
//                 if ((x + y) / 16 % 2 == 0) {
//                     buffer[y * Interface::WIDTH + x] = useAlternateColors ? Color{0, 255, 0} : Color{255, 0, 0};  // Green or Red
//                 } else {
//                     buffer[y * Interface::WIDTH + x] = useAlternateColors ? Color{255, 255, 0} : Color{0, 0, 255};  // Yellow or Blue
//                 }
//             }
//         }
//     }
// };
//

class GraphicsDemo1Stripe : public Screen {
public:
    GraphicsDemo1Stripe() : lastUpdateTime(std::chrono::steady_clock::now()), useAlternateColors(false), shift(0) {}

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
    }

    void handleEvent(const Event& event) override {
        if (event.type == EventType::KeyPress && event.value == GLFW_KEY_ENTER) {
            useAlternateColors = !useAlternateColors;
        }
    }

private:
    std::array<Color, Interface::WIDTH * Interface::HEIGHT> buffer;
    std::chrono::steady_clock::time_point lastUpdateTime;
    bool useAlternateColors;
    int shift;

    void updateAnimation() {
        shift = (shift + 1) % 32;  // Adjust this value for desired speed

        for (int y = 0; y < Interface::HEIGHT; ++y) {
            for (int x = 0; x < Interface::WIDTH; ++x) {
                if (((x + y + shift) / 16) % 2 == 0) {
                    buffer[y * Interface::WIDTH + x] = useAlternateColors ? Color{0, 255, 0} : Color{255, 0, 0};  // Green or Red
                } else {
                    buffer[y * Interface::WIDTH + x] = useAlternateColors ? Color{255, 255, 0} : Color{0, 0, 255};  // Yellow or Blue
                }
            }
        }
    }
};

} // namespace box
