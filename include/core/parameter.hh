#pragma once

#include <limits>
#include "core/util.hh"
#include "core/interface.hh"

namespace box {


template <typename T>
class Parameter {
public:
    Parameter(juce::CachedValue<T>& cv, te::AutomatableParameter* ap = nullptr)
        : value_(cv), ap_(ap) {
        if (!std::is_same_v<T, float> && ap_ != nullptr) {
            throw std::runtime_error{"Parameter::Component() created with non-float type has an AutomatableParameter"};
        }
        // value_.getValueSource().addListener(this); // TODO what does this do
    }
    // ~Parameter() {
    //     // value_.getValueSource().removeListener(this);
    // }

    void SetValue(T new_value) {
        value_ = new_value;
        if (ap_) { 
            ap_->setParameter(new_value, juce::NotificationType::dontSendNotification); 
        }
    }
    void SetNorm(T new_value) {
        if (ap_) { 
            ap_->setNormalisedParameter(new_value, juce::NotificationType::dontSendNotification); 
        } else {
            throw std::runtime_error{"Parameter::SetNorm() called on Component without AutomatableParameter"};
        }
        value_ = ap_->getCurrentValue();
    }
    T GetValue() const {
        if (ap_) { 
            return ap_->getCurrentValue(); 
        }
        return value_.get();
    }
    T GetNorm() const {
        if (ap_) { 
            return ap_->getCurrentNormalisedValue(); 
        } else {
            throw std::runtime_error{"Parameter::GetNorm() called on Component without AutomatableParameter"};
        }
        value_ = ap_->getCurrentValue();
    }

private:
    juce::CachedValue<T> &value_;
    te::AutomatableParameter *ap_;
};


} // namespace box
