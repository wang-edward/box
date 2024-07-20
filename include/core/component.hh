#pragma once
#include "core/util.hh"
#include "core/interface.hh"

namespace box {

template <typename T>
class Component {
public:
    Component(juce::CachedValue<T> &value, T min, T max, std::string name):
        value_{value}, min_{min}, max_{max}, name_{name} {
        // TODO check if this is good
        value_.setDefault(min_); 
    }
    void SetValue(T new_value) {
        if (min_ <= new_value && new_value <= max_) {
            value_ = new_value;
        } else {
            throw std::runtime_error {"Component::SetValue(" + std::to_string(new_value) + ") out of range: [" + min_ + ", " + max_ + "]"};
        }
    }
    T GetValue() const {
        return value_;
    }
    virtual void Render(Interface &interface) = 0;
    virtual void HandleEvent(const Event& event) = 0;
private:
    juce::CachedValue<T>& value_;
    T min_;
    T max_;
    std::string name_;
};

} // namespace box
