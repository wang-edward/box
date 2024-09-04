#pragma once
#include "core/util.hh"
#include "core/parameter.hh"
#include "graphics/shader.hh"
#include "graphics/mesh.hh"

namespace box {

template <typename T>
class Knob {
public:

    // TODO add min, max, name
    Knob(uint8_t x, uint8_t y, uint8_t radius, Color color, juce::CachedValue<T> &value, te::AutomatableParameter *ap = nullptr, std::string name = ""):
        x_{x}, y_{y}, radius_{radius}, color_{color}, param_{value, ap}, name_{name}
    {}

    virtual void Render(Interface &interface) {


        float percentage = param_.GetNorm();
        int radius = 18;

        int centerX = x_;
        int centerY = y_;

        // Angle to fill up to (in radians), starting from the 6 o'clock position and going counterclockwise
        float fillAngle = percentage * 2.0f * M_PI;

        for (int y = -radius; y <= radius; ++y) {
            for (int x = -radius; x <= radius; ++x) {
                // Check if point is inside the circle
                if (x * x + y * y <= radius * radius) {
                    // Calculate angle from center to this point
                    float angle = atan2(-y, x); // Inverting y to start from 6 o'clock
                    if (angle < 0) {
                        angle += 2.0f * M_PI;
                    }
                    // Draw pixel if within the fill angle
                    if (angle <= fillAngle) {
                        interface.DrawPixel(centerX + x, centerY + y, color_);
                    }
                }
            }
        }
    }
    virtual void HandleEvent(const Event& event) {

    }

    Parameter<T> param_;
private:
    uint8_t x_, y_, radius_;
    Color color_;
    std::string name_;
};

} // namespace box

