#pragma once
#include <cstdint>

#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion;

namespace box {

constexpr int NUM_TRACKS = 8;

struct Color {
    uint8_t r_, g_, b_;

    bool operator==(const Color& other) const {
        return r_ == other.r_ && g_ == other.g_ && b_ == other.b_;
    }

    bool operator!=(const Color& other) const {
        return !(*this == other);
    }
};

enum class DeviceType {
    Emulator,
    Hardware
};

enum class EventType {
    KeyPress,
    KeyRelease,
};

enum class ScreenType {
    GraphicsDemo1Stripe,
    GraphicsDemo2Bomb,
};

struct Event {
    EventType type_;
    int value_;
};

std::string to_string(ScreenType screenType);

} // namespace box
