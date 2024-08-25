#pragma once

#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <string>

#include <tracktion_engine/tracktion_engine.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif
#include <glm/glm.hpp>

namespace te = tracktion;

namespace box {


constexpr int NUM_TRACKS = 8;

enum class LogLevel {Off, Debug, Warn, Err};

LogLevel string_to_loglevel(const std::string& s);
std::string loglevel_to_string(LogLevel l);
LogLevel get_loglevel();

const LogLevel LOGLEVEL = get_loglevel();

void log_msg(LogLevel l, const std::string& msg);
#define LOG_MSG(l, msg) log_msg(l, msg)

template<typename T>
void log_var(const std::string &var_name, const T &var_value) {
    log_msg(LogLevel::Debug, var_name + ": " + std::to_string(var_value));
}
#define LOG_VAR(var) log_var(#var, var)

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
