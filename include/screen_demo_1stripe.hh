#pragma once
#include "interface.hh"
#include "screen.hh"
#include <iostream>
#include <chrono>
#include <array>

namespace box {

class GraphicsDemo1Stripe : public Screen {
public:
    GraphicsDemo1Stripe() : last_update_time_(std::chrono::steady_clock::now()), use_alternate_colors_(false), shift_(0) {}

    void render(Interface& interface) override {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update_time_).count();

        if (elapsed > 10) {  // Update animation every 100 ms
            last_update_time_ = now;
            update_animation();
        }

        for (int y = 0; y < Interface::HEIGHT; ++y) {
            for (int x = 0; x < Interface::WIDTH; ++x) {
                interface.draw_pixel(x, y, buffer_[y * Interface::WIDTH + x]);
            }
        }
    }

    void handle_event(const Event& event) {
        if (event.type_ == EventType::KeyPress && event.value_ == GLFW_KEY_ENTER) {
            use_alternate_colors_ = !use_alternate_colors_;
        }
    }

private:
    std::array<Color, Interface::WIDTH * Interface::HEIGHT> buffer_;
    std::chrono::steady_clock::time_point last_update_time_;
    bool use_alternate_colors_;
    int shift_;

    void update_animation() {
        shift_ = (shift_ + 1) % 32;  // Adjust this value for desired speed

        for (int y = 0; y < Interface::HEIGHT; ++y) {
            for (int x = 0; x < Interface::WIDTH; ++x) {
                if (((x + y + shift_) / 16) % 2 == 0) {
                    buffer_[y * Interface::WIDTH + x] = use_alternate_colors_ ? Color{0, 255, 0} : Color{255, 0, 0};  // Green or Red
                } else {
                    buffer_[y * Interface::WIDTH + x] = use_alternate_colors_ ? Color{255, 255, 0} : Color{0, 0, 255};  // Yellow or Blue
                }
            }
        }
    }
};

} // namespace box
