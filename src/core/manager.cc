#include "core/manager.hh"
#include "plugin/four_osc.hh"
#include "plugin/delay.hh"
#include "plugin/chorus.hh"
#include "plugin/reverb.hh"
#include <algorithm>

namespace box {

static void assert_tracks(const std::vector<std::unique_ptr<Track>> &tracks, size_t index, const std::string &name) {
    if (!(index >= 0 && index < MAX_TRACKS)) {
        throw std::runtime_error{"Manager::" + name + "(): index [" + std::to_string(index) + 
            "] out of range: [0, " + std::to_string(MAX_TRACKS) + "]"};
    }
}

Manager:: Manager(te::Edit &edit):
    edit_{edit},
    current_track_{0}, screen_state_{ScreenState::Timeline},
    base_tracks_{te::getAudioTracks(edit)} 
{
    AddTrack(); // ensure there's always at least 1
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
            // render timeline
            for (size_t i = 0; i < MAX_TRACKS; i++) {
                float x = 0;
                float y = (24 * i) + 32;
                float width = 128;
                float height = (24 * (i + 1)) + 32;
                DrawRectangleRec(Rectangle{x, y, width, height}, colors_[i]);
            }
            break;
        case ScreenState::Track:
            tracks_[current_track_]->Render(interface);
            break;
        case ScreenState::PluginSelector:
            for (size_t i = 0; i < interface.HEIGHT / 8; i++) {
                if (i >= sel_plugin_names_.size()) break;
                float x =  0;
                float y = 16 * i;
                float width = 128;
                float height = 15;
                Color color = RED;
                if (sel_current_index_ == i) {
                    color = BLUE;
                }
                DrawRectangleRec(Rectangle{x, y, width, height}, DARKGRAY);
                DrawText(sel_plugin_names_[i].c_str(), x, y, 5, color);
            }
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
                        case KEY_UP:
                            current_track_ = std::min(current_track_ + 1, clamp_decrement(tracks_.size()));
                            break;
                        case KEY_DOWN:
                            current_track_ = clamp_decrement(current_track_);
                            break;
                        case KEY_O: // add track
                            // assert?
                            if (tracks_.size() < MAX_TRACKS) {
                                AddTrack();
                            }
                            // AddTrack()
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
            switch(event.type) {
                case EventType::KeyPress:
                    switch (event.value) {
                        case KEY_K:
                            sel_current_index_ = clamp_decrement(sel_current_index_);
                            break;
                        case KEY_J:
                            sel_current_index_ = std::min(sel_current_index_ + 1, sel_plugin_names_.size() - 1);
                            break;
                        case KEY_ENTER:
                            te::Plugin *p = edit_.getPluginCache().createNewPlugin(sel_plugin_names_[sel_current_index_], {}).get();
                            switch(sel_current_index_) {
                                case 0:
                                    tracks_[current_track_]->AddPlugin(std::make_unique<FourOsc>(p));
                                    break;
                                case 1:
                                    tracks_[current_track_]->AddPlugin(std::make_unique<Chorus>(p));
                                    break;
                                case 2:
                                    tracks_[current_track_]->AddPlugin(std::make_unique<Reverb>(p));
                                    break;
                                case 3:
                                    tracks_[current_track_]->AddPlugin(std::make_unique<Delay>(p));
                                    break;
                            }
                            LOG_VAR(tracks_[current_track_]->plugins_.size());
                    }
            }
            break;
    }

}

} // namespace box
