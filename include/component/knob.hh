#pragma once
#include "core/util.hh"
#include "core/parameter.hh"
#include "core/cv.hh"

namespace box {

template <typename T>
struct Knob
{
    Parameter<T> param_;
    uint8_t x_, y_, radius_;
    Color color_;
    std::string name_;

    Knob(Parameter<T> p, uint8_t x, uint8_t y, uint8_t radius, Color color, std::string name = "")
    : param_{p}, x_{x}, y_{y}, radius_{radius}, color_{color}, name_{name}
    {}

    virtual void Render(Interface &interface) 
    {
        auto angle = param_.GetNorm() * 360;
        auto x = static_cast<float>(x_);
        auto y = static_cast<float>(y_);
        DrawCircleSector(Vector2{x, y}, radius_, 0.f, angle, 360, color_);

        const int font_size = 10;
        int width = MeasureText(name_.c_str(), font_size);
        DrawText(name_.c_str(), x - (width / 2), y + 20, font_size, WHITE);
    }
    virtual void HandleEvent(const Event& event) 
    {
        
    }
};

} // namespace box

