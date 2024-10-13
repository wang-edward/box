#include "core/timeline.hh"
#include "core/manager.hh"

namespace box {

void Timeline:: Render(Interface &interface) {
    for (size_t i = 0; i < MANAGER->tracks_.size(); i++) {
        float x = 0;
        float y = (24 * i) + 32;
        float width = 128;
        float height = 24;
        DrawRectangleRec(Rectangle{x, y, width, height}, MANAGER->colors_[i]);
    }

    // render active track
    {
        float x = 0;
        float y = (24 * MANAGER->current_track_) + 32;
        float width = 128;
        float height = 24;
        DrawRectangleRec(Rectangle{x, y, width, height}, {0xff, 0xff, 0xff, 0x80});
    }
}

void Timeline:: HandleEvent(const Event &event) {
    switch (event.type) {
        case EventType::KeyPress:
            switch (event.value) {
                case KEY_ENTER:
                    MANAGER->screen_state_ = Manager::ScreenState::Track;
                    break;
                case KEY_J:
                    MANAGER->current_track_ = std::min(MANAGER->current_track_ + 1, clamp_decrement(MANAGER->tracks_.size()));
                    break;
                case KEY_K:
                    MANAGER->current_track_ = clamp_decrement(MANAGER->current_track_);
                    break;
                case KEY_O: // add track
                    if (MANAGER->tracks_.size() < MAX_TRACKS) {
                        MANAGER->AddTrack();
                    }
                    break;
                case KEY_A: // add plugin
                    MANAGER->screen_state_ = Manager::ScreenState::PluginSelector;
                    break;
            }
            break;
    }
}

} // namespace box
