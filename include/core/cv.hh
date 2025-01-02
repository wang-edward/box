#pragma once

#include <algorithm>

#include "core/util.hh"

template <typename T> struct CV {
    juce::CachedValue<T> &value_;
    T min_, max_;

    // TODO add default value?
    CV(juce::CachedValue<T> &cv, T min, T max)
        : value_{cv}, min_{min}, max_{max} {
        assert(min <= max);
    }

    void SetValue(T new_value) { value_ = std::clamp(new_value, min_, max_); }

    void SetNorm(T norm) {
        norm = std::clamp(norm, static_cast<T>(0), static_cast<T>(1));
        const auto range = max_ - min_;
        value_ = range * norm;
    }

    T GetValue() const { return value_.get(); }
    T GetNorm() const {
        const auto range = max_ - min_;
        return value_ / range;
    }
};
