#pragma once
#include "core/util.hh"
#include "core/parameter.hh"
#include "core/cv.hh"

namespace box {

template <typename T>
class Knob {
public:

    // TODO add min, max, name
    // Knob(uint8_t x, uint8_t y, uint8_t radius, Color color, juce::CachedValue<T> &value, te::AutomatableParameter *ap = nullptr, std::string name = ""):
    //     x_{x}, y_{y}, radius_{radius}, color_{color}, param_{value, ap}, name_{name}
    // {}

    Knob(CV<T> cv)
    : cv_{cv}, x_{32}, y_{32}, radius_{16}, color_{GREEN}, name_{"yoo"}
    {}

    virtual void Render(Interface &interface) 
    {
        // auto angle = param_.GetNorm() * 360;
        auto angle = cv_.GetNorm() * 360;
        auto x = static_cast<float>(x_);
        auto y = static_cast<float>(y_);
        DrawCircleSector(Vector2{x, y}, radius_, 0.f, angle, 360, GREEN);
    }
    virtual void HandleEvent(const Event& event) 
    {
        
    }

    CV<T> cv_;
    // Parameter<T> param_;
private:
    uint8_t x_, y_, radius_;
    Color color_;
    std::string name_;
};

} // namespace box

