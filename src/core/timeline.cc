#include "core/timeline.hh"
#include "core/app.hh"

namespace box {

void Timeline:: Render(Interface &interface) {
    for (size_t i = 0; i < APP->tracks_.size(); i++) {
        float x = 0;
        float y = (24 * i) + 32;
        float width = 128;
        float height = 24;
        DrawRectangleRec(Rectangle{x, y, width, height}, APP->colors_[i]);
    }

    // render active track
    {
        float x = 0;
        float y = (24 * APP->current_track_) + 32;
        float width = 128;
        float height = 24;
        DrawRectangleRec(Rectangle{x, y, width, height}, {0xff, 0xff, 0xff, 0x80});
    }
}

void Timeline:: HandleEvent(const Event &event) {
    switch (screen_state_) {
    case ScreenState::Overview:
        switch (event.type) {
        case EventType::KeyPress:
            switch (event.value) {
            case KEY_ENTER:
                APP->screen_state_ = App::ScreenState::Track;
                break;
            case KEY_J:
                APP->current_track_ = std::min(APP->current_track_ + 1, clamp_decrement(APP->tracks_.size()));
                break;
            case KEY_K:
                APP->current_track_ = clamp_decrement(APP->current_track_);
                break;
            case KEY_O: // add track
                if (APP->tracks_.size() < MAX_TRACKS) {
                    APP->AddTrack();
                }
                break;
            }
            break;
        }
        break;
    }
}

} // namespace box
