#pragma once

#include <cstdint>

#include <tracktion_engine/tracktion_engine.h>
#include <GLFW/glfw3.h>

namespace te = tracktion;

namespace box {


constexpr int NUM_TRACKS = 8;

// Template logging function
template<typename T>
void log_var(const std::string& varName, const T& varValue) {
    std::cout << varName << ": " << varValue << std::endl;
}

// Macro to simplify logging
#define LOG_VAR(var) log_var(#var, var)

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
