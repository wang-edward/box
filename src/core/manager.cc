#include "core/manager.hh"
#include <algorithm>

namespace box {

static void assert_tracks(const std::vector<std::unique_ptr<Track>> &tracks, size_t index, const std::string &name) {
    if (!(index >= 0 && index < NUM_TRACKS)) {
        throw std::runtime_error{"Manager::" + name + "(): index [" + std::to_string(index) + 
            "] out of range: [0, " + std::to_string(NUM_TRACKS) + "]"};
    }
}

size_t clamp_decrement(size_t x) {
    if (x == 0) return 0;
    else return x - 1;
}

Manager:: Manager() : current_track_{0}, screen_state_{ScreenState::Timeline} {
}

void Manager:: AddTrack(std::unique_ptr<Track> track) {
    tracks_.push_back(std::move(track));
}

void Manager:: SetCurrentTrack(size_t track_index) {
    if (track_index >= 0 && track_index < NUM_TRACKS) {
        current_track_ = track_index;
    }
}

void Manager:: Render(Interface& interface) {
    assert_tracks(tracks_, current_track_, "render");
    switch (screen_state_) {
        case ScreenState::Timeline:
            // render timeline
            DrawText("TIMELINE", 10, 10, 5, DARKGRAY);

            break;
        case ScreenState::Track:
            tracks_[current_track_]->Render(interface);
            break;
    }
}

void Manager:: HandleEvent(const Event& event) {
    std::cout << "in" << std::endl;
    assert_tracks(tracks_, current_track_, "handleEvent");
    switch (screen_state_) {
        case ScreenState::Timeline:
            switch (event.type) {
                case EventType::KeyPress:
                    switch (event.value) {
                        case KEY_ENTER:
                            screen_state_ = ScreenState::Track;
                            break;
                        case KEY_UP:
                            current_track_ = std::min(current_track_ + 1, tracks_.size() - 1);
                            break;
                        case KEY_DOWN:
                            current_track_ = clamp_decrement(current_track_);
                            break;
                    }
                    break;
            }
            LOG_VAR(current_track_);
            break;
        case ScreenState::Track:
            if (event.type == EventType::KeyPress && event.value == KEY_ESCAPE) {
                screen_state_ = ScreenState::Timeline;
            } else {
                tracks_[current_track_]->HandleEvent(event);
            }
            break;
    }

}

} // namespace box
