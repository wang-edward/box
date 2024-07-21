#pragma once
#include "core/component.hh"

namespace box {

template <typename T>
class Knob : public Component<T> {
public:

    // TODO add min, max, name
    Knob(int x, int y, juce::CachedValue<T> &value) :
        x_{x}, y_{y}, Component<T>{value} {}

    virtual void Render(Interface &interface) override {
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                interface.DrawPixel(x_ + i, y_ + j, {0, 255, 0});
            }
        }
    }
    virtual void HandleEvent(const Event& event) override {

    }
private:
    int x_;
    int y_;
};

} // namespace box

