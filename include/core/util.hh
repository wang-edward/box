#pragma once

#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <string>

#include "raylib.h"
#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion;

namespace box {

struct App;
extern App *APP;

constexpr size_t MAX_ROWS = 4;
constexpr size_t MAX_TRACKS = 8;
constexpr size_t ICON_RADIUS = 8;
constexpr size_t ICON_WIDTH = 16;

void assert_multiple(float a, float b);
bool is_close(float a, float b);

size_t clamp_decrement(size_t x);
size_t clamp_increment(size_t x, size_t max);

enum class LogLevel {Off, Debug, Warn, Err};
LogLevel string_to_loglevel(const std::string& s);
std::string loglevel_to_string(LogLevel l);
LogLevel get_loglevel();
const LogLevel LOGLEVEL = get_loglevel();
void LOG_MSG(const std::string& msg, LogLevel l = LogLevel::Debug);
template<typename T>
void log_var(const std::string &var_name, const T &var_value) {
    LOG_MSG(var_name + ": " + std::to_string(var_value), LogLevel::Debug);
}
#define LOG_VAR(var) log_var(#var, var)

enum class DeviceType {
    Emulator,
    Hardware
};

enum class EventType {
    KeyPress,
    KeyRelease,
    PotDown,
    PotUp,
};

struct Event {
    EventType type;
    int value;
};

const std::unordered_map<int, int> KEY_TO_MIDI = {
    {KEY_A, 60},  // C4
    {KEY_S, 62},  // D4
    {KEY_D, 64},  // E4
    {KEY_F, 65},  // F4
    {KEY_G, 67},  // G4
    {KEY_H, 69},  // A4
    {KEY_J, 71},  // B4
    {KEY_K, 72},  // C5
    {KEY_L, 74}   // D5
};

} // namespace box
