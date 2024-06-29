#include "io_interface.hh"

#include <array>
#include <condition_variable>
#include <mutex>
#include <ncurses.h>
#include <queue>
#include <stdexcept>

IOInterface:: IOInterface() {
    activeBuffer.fill(Color{0, 0, 0});
    inactiveBuffer.fill(Color{0, 0, 0});
    if constexpr (DisplayType == DeviceType::EMULATOR) {
        initscr();
        start_color();
        initializeColors();
        curs_set(0); // Hide the cursor
        cbreak();
        noecho();
    } else {
        // Initialize SPI display here
    }
    if constexpr (InputType == DeviceType::EMULATOR) {
        // Ncurses input initialization is already handled in ncurses initialization
    } else {
        // Initialize microcontroller input handling (e.g., serial interface)
    }
}

IOInterface:: ~IOInterface() {
    if constexpr (DisplayType == DeviceType::EMULATOR) {
        endwin();
    } else {
        // Clean up SPI display resources
    }
}

void IOInterface:: drawPixel(int x, int y, Color color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        inactiveBuffer[y * WIDTH + x] = color;
    }
}

void IOInterface:: swapBuffers() { 
    std::swap(activeBuffer, inactiveBuffer); 
}

void IOInterface:: drawToScreen() const {
    if constexpr (DisplayType == DeviceType::EMULATOR) {
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                Color color = activeBuffer[y * WIDTH + x];
                int colorNumber = (color.r << 12) | (color.g << 6) | color.b;
                attron(COLOR_PAIR(colorNumber));
                mvaddch(y, x, ' '); // Each pixel is represented by a space character
                attroff(COLOR_PAIR(colorNumber));
            }
        }
        refresh();
    } else {
        // Send activeBuffer to the SPI display
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                Color color = activeBuffer[y * WIDTH + x];
                // write color to SPI display at (x, y)
            }
        }
    }
}

bool IOInterface:: pollEvent(Event &event) {
    if constexpr (InputType == DeviceType::EMULATOR) {
        int ch = getch();
        if (ch != ERR) {
            event.type = EventType::MIDI_MESSAGE;
            event.key = ch;
            return true;
        }
        return false;
    } else {
        // poll input from mcu via serial interface
    }
}
