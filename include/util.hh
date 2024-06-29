#pragma once
#include <cstdlib>
#include <ncurses.h>
#include <stdexcept>

struct Color {
    uint8_t r, g, b;
};

enum class DeviceType {EMULATOR, HARDWARE};

enum class EventType {
    MIDI_MESSAGE,
    PLAY,
    PAUSE,
};

struct Event {
    EventType type;
    int key;
};

enum class ScreenType {
    MAIN,
};

// Utility function to initialize 18-bit RGB colors in ncurses
void initializeColors() {
    if (!can_change_color() || COLORS < 256) {
        endwin();
        throw std::runtime_error("Terminal does not support extended colors");
    }

    for (int r = 0; r < 64; ++r) {
        for (int g = 0; g < 64; ++g) {
            for (int b = 0; b < 64; ++b) {
                int colorNumber = (r << 12) | (g << 6) | b;
                init_color(colorNumber, r * 4, g * 4, b * 4);
                init_pair(colorNumber, colorNumber, colorNumber);
            }
        }
    }
}
