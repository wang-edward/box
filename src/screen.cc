#include <ncurses.h>
#include <vector>

// Define the size of the display
constexpr int WIDTH = 128;
constexpr int HEIGHT = 128;

// Initialize the display buffer
std::vector<std::vector<int>> displayBuffer(HEIGHT, std::vector<int>(WIDTH, 0));

// Function to initialize ncurses and set up color pairs
void initializeNcurses() {
    initscr();
    start_color();
    // Initialize color pairs (assuming 8 colors for simplicity)
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_RED);
    init_pair(3, COLOR_GREEN, COLOR_GREEN);
    init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(5, COLOR_BLUE, COLOR_BLUE);
    init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(7, COLOR_CYAN, COLOR_CYAN);
    init_pair(8, COLOR_WHITE, COLOR_WHITE);

    // Hide the cursor
    curs_set(0);
    // No line buffering
    cbreak();
    // Disable echoing
    noecho();
}

// Function to draw a pixel on the display buffer
void drawPixel(int x, int y, int color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        displayBuffer[y][x] = color;
    }
}

// Function to update the ncurses display
void updateDisplay() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int color = displayBuffer[y][x];
            attron(COLOR_PAIR(color));
            mvaddch(y, x, ' '); // Each pixel is represented by a space character
            attroff(COLOR_PAIR(color));
        }
    }
    refresh();
}

int main() {
    // Initialize ncurses
    initializeNcurses();

    // Draw some pixels for demonstration purposes
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int color = (x + y) % 8 + 1; // Cycle through colors
            drawPixel(x, y, color);
        }
    }

    // Update the display
    updateDisplay();

    // Wait for user input before exiting
    getch();

    // End ncurses mode
    endwin();
    return 0;
}
