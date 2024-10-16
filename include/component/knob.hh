#pragma once
#include "core/util.hh"
#include "core/parameter.hh"

namespace box {

template <typename T>
class Knob {
public:

    // TODO add min, max, name
    Knob(uint8_t x, uint8_t y, uint8_t radius, Color color, juce::CachedValue<T> &value, te::AutomatableParameter *ap = nullptr, std::string name = ""):
        x_{x}, y_{y}, radius_{radius}, color_{color}, param_{value, ap}, name_{name}
    {}

    virtual void Render(Interface &interface) 
    {
        // DrawCircle(32, 32, 5, GREEN);
        auto angle = param_.GetNorm() * 360;
        DrawCircleSector(Vector2{32, 32}, 5, 0.f, angle, 360, GREEN);
        // void DrawCircleSector(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color); 
    }
    virtual void HandleEvent(const Event& event) 
    {
        
    }

    Parameter<T> param_;
private:
    uint8_t x_, y_, radius_;
    Color color_;
    std::string name_;
};

} // namespace box

