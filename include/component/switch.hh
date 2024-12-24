#pragma once
#include "core/util.hh"
#include "core/parameter.hh"

// instead of using cv.hh, we directly use juce::CachedValue<bool>

namespace box{
struct Switch{
    juce::CachedValue<bool> &value_;
    uint8_t x_, y_;
    Color colour_;
    std::string name_;

    Switch(juce::CachedValue<bool> &value, uint8_t x, uint8_t y, std::string name="")
    : value_{value}, x_{x}, y_{y}, name_{name}, colour_{RED}
    {}

    virtual void Render(Interface &interface) 
    {
        colour_ = value? RED : WHITE;
        const int font_size = 10;
        int width = MeasureText(name_.c_str(), font_size);
        DrawText(name_.c_str(), x - (width / 2), y + 20, font_size, colour_);
    }

    virtual void HandleEvent(const Event& event)
    {

    }
};

} // namespace box