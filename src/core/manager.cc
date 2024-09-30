#include "core/manager.hh"
#include <algorithm>

namespace box {

static void assert_tracks(const std::vector<std::unique_ptr<Track>> &tracks, size_t index, const std::string &name) {
    if (tracks.size() == 0 || index >= tracks.size()) {
        throw std::runtime_error{"Manager::" + name + "(): index [" + std::to_string(index) + 
            "] out of range: [0, " + std::to_string(tracks.size()) + "]"};
    }
}

Manager:: Manager(te::Edit &edit):
    edit_{edit},
    current_track_{0}, screen_state_{ScreenState::Timeline},
    base_tracks_{te::getAudioTracks(edit)},
    plugin_sel_{plugin_sel_callback_}
{
    AddTrack(); // ensure there's always at least 1
    LOG_VAR(tracks_.size());
}

void Manager:: AddTrack() {
    tracks_.push_back(std::make_unique<Track>(*base_tracks_[tracks_.size()]));
}

void Manager:: SetCurrentTrack(size_t track_index) {
    if (track_index >= 0 && track_index < MAX_TRACKS) {
        current_track_ = track_index;
    }
}

void Manager:: Render(Interface& interface) {
    assert_tracks(tracks_, current_track_, "render");
    switch (screen_state_) {
        case ScreenState::Timeline:
            // render tracks
            for (size_t i = 0; i < tracks_.size(); i++) {
                float x = 0;
                float y = (24 * i) + 32;
                float width = 128;
                float height = 24;
                DrawRectangleRec(Rectangle{x, y, width, height}, colors_[i]);
            }

            // render active track
            {
                float x = 0;
                float y = (24 * current_track_) + 32;
                float width = 128;
                float height = 24;
                DrawRectangleRec(Rectangle{x, y, width, height}, {0xff, 0xff, 0xff, 0x80});
            }
            break;
        case ScreenState::Track:
            tracks_[current_track_]->Render(interface);
            break;
        case ScreenState::PluginSelector:
            plugin_sel_.Render(interface);
            break;
    }
}

void Manager:: HandleEvent(const Event& event) {
    assert_tracks(tracks_, current_track_, "handleEvent");
    switch (screen_state_) {
        case ScreenState::Timeline:
            switch (event.type) {
                case EventType::KeyPress:
                    switch (event.value) {
                        case KEY_ENTER:
                            screen_state_ = ScreenState::Track;
                            break;
                        case KEY_J:
                            current_track_ = std::min(current_track_ + 1, clamp_decrement(tracks_.size()));
                            break;
                        case KEY_K:
                            current_track_ = clamp_decrement(current_track_);
                            break;
                        case KEY_O: // add track
                            if (tracks_.size() < MAX_TRACKS) {
                                AddTrack();
                            }
                            break;
                        case KEY_A: // add plugin
                            screen_state_ = ScreenState::PluginSelector;
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
        case ScreenState::PluginSelector:
            if (event.type == EventType::KeyPress && event.value == KEY_ESCAPE) {
                screen_state_ = ScreenState::Timeline;
                return;
            }
            plugin_sel_.HandleEvent(event);
            break;
    }

}

} // namespace box
