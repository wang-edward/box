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
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                interface.DrawPixel(x_ + i, y_ + j, {0, 255, 0});
            }
        }
    }
    virtual void HandleEvent(const Event& event) {

    }

    Parameter<T> param_;
private:
    int x_, y_;
    std::string name_;
};

} // namespace box

