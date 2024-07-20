#pragma once
#include <tracktion_engine/tracktion_engine.h>
#include <GLFW/glfw3.h>

#include <cstdint>

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

struct Event {
    EventType type;
    int value;
};

const std::unordered_map<int, int> KEY_TO_MIDI = {
    {GLFW_KEY_A, 60},  // C4
    {GLFW_KEY_S, 62},  // D4
    {GLFW_KEY_D, 64},  // E4
    {GLFW_KEY_F, 65},  // F4
    {GLFW_KEY_G, 67},  // G4
    {GLFW_KEY_H, 69},  // A4
    {GLFW_KEY_J, 71},  // B4
    {GLFW_KEY_K, 72},  // C5
    {GLFW_KEY_L, 74}   // D5
};

} // namespace box
