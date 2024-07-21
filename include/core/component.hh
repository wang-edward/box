#pragma once

#include <limits>
#include "core/util.hh"
#include "core/interface.hh"

namespace box {

template <typename T>
class Component {
public:
    Component(juce::CachedValue<T> &value, te::AutomatableParameter::Ptr ap, std::string name = ""):
        value_{value}, ap_{ap}, name_{name} {}
    void SetValue(T new_value) {
        value_ = new_value; // TODO some check for range clip
        if (ap_) {
            ap_->setParameter(new_value, juce::sendNotification);
        }
    }
    void SetNorm(float new_value) {
        if (!std::is_same_v<T, float>) {
            throw std::runtime_error{"Component::SetNorm() called on non-float"};
        }
        if (ap_) {
            ap_->setParameter(new_value, juce::sendNotification);
        } else {
            throw std::runtime_error{"Component::SetNorm() called on [" + name_ + "] without AutomatableParameter"};
        }
        
    }
    T GetValue() const {
        return value_;
    }
    virtual void Render(Interface &interface) = 0;
    virtual void HandleEvent(const Event& event) = 0;
private:
    juce::CachedValue<T>& value_;
    te::AutomatableParameter::Ptr ap_;
    std::string name_;
};

} // namespace box
