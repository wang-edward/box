#pragma once
#include "screen.hh"
#include "interface.hh"
#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>

namespace box {

class GraphicsDemo2Bomb : public Screen {
public:
    GraphicsDemo2Bomb() : last_update_time_(std::chrono::steady_clock::now()) {}

    void Render(Interface& interface) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update_time_).count();

        if (elapsed > 50) {  // Update animation every 50 ms
            last_update_time_ = now;
            UpdateAnimation();
        }

        for (int y = 0; y < Interface::HEIGHT; ++y) {
            for (int x = 0; x < Interface::WIDTH; ++x) {
                interface.DrawPixel(x, y, buffer_[y * Interface::WIDTH + x]);
            }
        }
    }

    void HandleEvent(const Event& event) {
        std::cout << "Handled event: ";
        if (event.type_ == EventType::KeyPress) {
            std::cout << "KeyPress, value: " << event.value_ << std::endl;
        } else if (event.type_ == EventType::KeyRelease) {
            std::cout << "KeyRelease, value: " << event.value_ << std::endl;
        }
    }

private:
    std::array<Color, Interface::WIDTH * Interface::HEIGHT> buffer_;
    std::chrono::steady_clock::time_point last_update_time_;

    struct Explosion {
        int x_, y_;
        int radius_;
        Color color_;
    };

    std::vector<Explosion> explosions_;

    void UpdateAnimation() {
        // Clear buffer
        buffer_.fill(Color{0, 0, 0});

        // Add a new explosion at random intervals
        if (rand() % 10 == 0) {
            int x = rand() % Interface::WIDTH;
            int y = rand() % Interface::HEIGHT;
            explosions_.push_back({x, y, 1, {255, static_cast<uint8_t>(rand() % 256), 0}});
        }

        // Update and draw explosions
        for (auto& explosion : explosions_) {
            DrawExplosion(explosion);
            explosion.radius_++;
        }

        // Remove explosions that are too large
        explosions_.erase(std::remove_if(explosions_.begin(), explosions_.end(), [](const Explosion& e) {
            return e.radius_ > Interface::WIDTH / 2;
        }), explosions_.end());
    }

    void DrawExplosion(const Explosion& explosion) {
        int x0 = explosion.x_;
        int y0 = explosion.y_;
        int radius = explosion.radius_;
        Color color = explosion.color_;

        for (int y = -radius; y <= radius; ++y) {
            for (int x = -radius; x <= radius; ++x) {
                if (x * x + y * y <= radius * radius) {
                    int px = x0 + x;
                    int py = y0 + y;
                    if (px >= 0 && px < Interface::WIDTH && py >= 0 && py < Interface::HEIGHT) {
                        buffer_[py * Interface::WIDTH + px] = color;
                    }
                }
            }
        }
    }
};

} // namespace box
