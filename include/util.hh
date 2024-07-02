#pragma once
#include <cstdint>

#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion;

struct Color {
    uint8_t r, g, b;

    bool operator==(const Color& other) const {
        return r == other.r && g == other.g && b == other.b;
    }

    bool operator!=(const Color& other) const {
        return !(*this == other);
    }
};

enum class DeviceType {
    EMULATOR,
    HARDWARE
};

enum class EventType {
    KEYPRESS,
    KEYRELEASE,
};

struct Event {
    EventType type;
    int value;
};
