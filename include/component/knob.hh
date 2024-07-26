#pragma once
#include "core/parameter.hh"

namespace box {

template <typename T>
class Knob {
public:

    // TODO add min, max, name
    Knob(int x, int y, juce::CachedValue<T> &value, te::AutomatableParameter *ap = nullptr, std::string name = ""):
        x_{x}, y_{y}, param_{value, ap}, name_{name} {}

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
                        interface.DrawPixel(centerX + x, centerY + y, {255, 0, 0});
                    }
                }
            }
    }


        // for (int i = 0; i < 16; i++) {
        //     for (int j = 0; j < 16; j++) {
        //         interface.DrawPixel(x_ + i, y_ + j, {0, 255, 0});
        //     }
        // }
    }
    virtual void HandleEvent(const Event& event) {

    }

    Parameter<T> param_;
private:
    int x_, y_;
    std::string name_;
};

} // namespace box

