#include "screen.hh"
enum class ScreenType {MAIN}

class ScreenManager {
    std::map<ScreenType, std::unique_ptr<Screen>> screens_;
    ScreenType activeScreen_;

public:
    ScreenManager();
    void addScreen(ScreenType type, std::unique_ptr<Screen> screen);
    void setActiveScreen(ScreenType type);
    void render();
    void handleEvent(const Event &event);
};
