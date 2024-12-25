#pragma once
#include "core/parameter.hh"
#include "core/util.hh"

// instead of using cv.hh, we directly use juce::CachedValue<bool>

namespace box {

struct Switch {
  juce::CachedValue<bool> &value_;
  uint8_t x_, y_;
  Color color_;
  std::string name_;

  Switch(juce::CachedValue<bool> &value, uint8_t x, uint8_t y, Color color,
         std::string name)
      : value_{value}, x_{x}, y_{y}, color_{color}, name_{name} {}

  virtual void Render(Interface &interface) {
    Color text_color = value_ ? color_ : WHITE;
    const int font_size = 10;
    int width = MeasureText(name_.c_str(), font_size);
    DrawText(name_.c_str(), x - (width / 2), y + 20, font_size, text_color);
  }

  virtual void HandleEvent(const Event &event) {}
};

}  // namespace box
