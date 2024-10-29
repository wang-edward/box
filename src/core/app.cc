#include "core/app.hh"
#include <algorithm>

namespace box {

static void assert_tracks(const std::vector<std::unique_ptr<Track>> &tracks, size_t index, const std::string &name) 
{
    if (tracks.size() == 0 || index >= tracks.size()) 
    {
        throw std::runtime_error{"App::" + name + "(): index [" + std::to_string(index) + 
            "] out of range: [0, " + std::to_string(tracks.size()) + "]"};
    }
}

App:: App(te::Edit &edit):
    edit_{edit},
    current_track_{0},
    base_tracks_{te::getAudioTracks(edit)}
{
    AddTrack(); // ensure there's always at least 1
    LOG_VAR(tracks_.size());
}

void App:: AddTrack()
{
    tracks_.push_back(std::make_unique<Track>(*base_tracks_[tracks_.size()]));
}

void App:: SetCurrentTrack(size_t track_index) 
{
    if (track_index >= 0 && track_index < MAX_TRACKS) 
    {
        current_track_ = track_index;
    }
}

void App:: Render(Interface& interface) 
{
    assert_tracks(tracks_, current_track_, "render");

    switch (screen_state_) 
    {
    case ScreenState::Timeline:
        timeline_.Render(interface);
        break;
    case ScreenState::Track:
        tracks_[current_track_]->Render(interface);
        break;
    case ScreenState::PluginSelector:
        plugin_sel_.Render(interface);
        break;
    }

    if (mode_ == Mode::Insert)
    {
        DrawRectangleLines(0, 0, 128, 127, PURPLE); // -1 so bottom row shows
    }
    if (edit_.getTransport().isRecording())
    {
        DrawRectangleLines(1, 1, 126, 125, RED); // -1 so bottom row shows
    }
}

void App:: HandleEvent(const Event& event) 
{
    assert_tracks(tracks_, current_track_, "handleEvent");
    if (event.type == EventType::KeyPress && event.value == KEY_I) 
    {
        if (mode_ == Mode::Normal) 
        {
            mode_ = Mode::Insert;
        } 
        else if (mode_ == Mode::Insert) 
        {
            mode_ = Mode::Normal;
        }
    }

    switch (screen_state_) 
    {
    case ScreenState::Timeline:
        timeline_.HandleEvent(event);
        break;
    case ScreenState::Track:
        tracks_[current_track_]->HandleEvent(event);
        break;
    case ScreenState::PluginSelector:
        plugin_sel_.HandleEvent(event);
        break;
    }

}

} // namespace box
