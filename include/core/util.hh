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
constexpr int DEFAULT_FONT_SIZE = 10;
constexpr int ICON_RADIUS = 8;
constexpr int ICON_WIDTH = 16;
constexpr int ROW_HEIGHT = 24;
constexpr int ROW_WIDTH = 128;
constexpr int HEADER_HEIGHT = 32;
constexpr int HEADER_WIDTH = 128;
constexpr int HEADER_FONT_SIZE = 10;
constexpr int SCREEN_HEIGHT = 128;
constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HALF = 64;
constexpr int SCREEN_QUARTER = 32;
constexpr int SCREEN_EIGHTH = 16;
// TODO replace HEIGHT and WIDTH with SCREEN_{}

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
    {KEY_W, 61},  // C#4
    {KEY_S, 62},  // D4
    {KEY_E, 63},  // D#4
    {KEY_D, 64},  // E4
    {KEY_F, 65},  // F4
    {KEY_T, 66},  // F#4
    {KEY_G, 67},  // G4
    {KEY_Y, 68},  // G#4
    {KEY_H, 69},  // A4
    {KEY_U, 70},  // A#4
    {KEY_J, 71},  // B4
    {KEY_K, 72},  // C5
    {KEY_O, 73},  // C#5
    {KEY_L, 74}   // D5
};

} // namespace box
