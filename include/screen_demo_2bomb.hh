#include "screen.hh"
#include "interface.hh"
#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>

namespace box {

class GraphicsDemo2Bomb : public Screen {
public:
    GraphicsDemo2Bomb() : lastUpdateTime(std::chrono::steady_clock::now()) {}

    void render(Interface& interface) override {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime).count();

        if (elapsed > 50) {  // Update animation every 50 ms
            lastUpdateTime = now;
            updateAnimation();
        }

        for (int y = 0; y < Interface::HEIGHT; ++y) {
            for (int x = 0; x < Interface::WIDTH; ++x) {
                interface.drawPixel(x, y, buffer[y * Interface::WIDTH + x]);
            }
        }
    }

    void handleEvent(te::Edit &edit, const Event& event) override {
        std::cout << "Handled event: ";
        if (event.type == EventType::KeyPress) {
            std::cout << "KeyPress, value: " << event.value << std::endl;
        } else if (event.type == EventType::KeyRelease) {
            std::cout << "KeyRelease, value: " << event.value << std::endl;
        }
    }

private:
    std::array<Color, Interface::WIDTH * Interface::HEIGHT> buffer;
    std::chrono::steady_clock::time_point lastUpdateTime;

    struct Explosion {
        int x, y;
        int radius;
        Color color;
    };

    std::vector<Explosion> explosions;

    void updateAnimation() {
        // Clear buffer
        buffer.fill(Color{0, 0, 0});

        // Add a new explosion at random intervals
        if (rand() % 10 == 0) {
            int x = rand() % Interface::WIDTH;
            int y = rand() % Interface::HEIGHT;
            explosions.push_back({x, y, 1, {255, static_cast<uint8_t>(rand() % 256), 0}});
        }

        // Update and draw explosions
        for (auto& explosion : explosions) {
            drawExplosion(explosion);
            explosion.radius++;
        }

        // Remove explosions that are too large
        explosions.erase(std::remove_if(explosions.begin(), explosions.end(), [](const Explosion& e) {
            return e.radius > Interface::WIDTH / 2;
        }), explosions.end());
    }

    void drawExplosion(const Explosion& explosion) {
        int x0 = explosion.x;
        int y0 = explosion.y;
        int radius = explosion.radius;
        Color color = explosion.color;

        for (int y = -radius; y <= radius; ++y) {
            for (int x = -radius; x <= radius; ++x) {
                if (x * x + y * y <= radius * radius) {
                    int px = x0 + x;
                    int py = y0 + y;
                    if (px >= 0 && px < Interface::WIDTH && py >= 0 && py < Interface::HEIGHT) {
                        buffer[py * Interface::WIDTH + px] = color;
                    }
                }
            }
        }
    }
};

} // namespace box
