#pragma once

#include <limits>
#include "core/util.hh"
#include "core/interface.hh"
#include "core/cv.hh"

namespace box {

using AP = te::AutomatableParameter;

template <typename T>
class Parameter {
    std::variant<CV<T>, AP*> param_;
public:
    Parameter(CV<T> cv)
    : param_{cv}
    {}

    Parameter(AP *p)
    : param_{p}
    {}

    void SetValue(T new_value)
    {
        if (std::holds_alternative<CV<T>>(param_))
        {
            auto v = std::get<CV<T>>(param_);
            v.SetValue(new_value);
        }
        else if (std::holds_alternative<AP*>(param_))
        {
            auto v = std::get<AP*>(param_);
            v->setParameter(new_value, juce::NotificationType::dontSendNotification);
        }
    }
    void SetNorm(T new_value)
    {
        if (std::holds_alternative<CV<T>>(param_))
        {
            auto v = std::get<CV<T>>(param_);
            v.SetNorm(new_value);
        }
        else if (std::holds_alternative<AP*>(param_))
        {
            auto v = std::get<AP*>(param_);
            v->setNormalisedParameter(new_value, juce::NotificationType::dontSendNotification);
        }
    }
    T GetValue() const
    {
        if (std::holds_alternative<CV<T>>(param_))
        {
            auto v = std::get<CV<T>>(param_);
            return v.GetValue();
        }
        else if (std::holds_alternative<AP*>(param_))
        {
            auto v = std::get<AP*>(param_);
            return v->getCurrentValue();
        }
    }
    T GetNorm() const
    {
        if (std::holds_alternative<CV<T>>(param_))
        {
            auto v = std::get<CV<T>>(param_);
            return v.GetNorm();
        }
        else if (std::holds_alternative<AP*>(param_))
        {
            auto v = std::get<AP*>(param_);
            return v->getCurrentNormalisedValue();
        }
    }
};


} // namespace box
