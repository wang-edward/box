#pragma once
#include "core/util.hh"
#include "core/track.hh"

namespace box {

class Manager {
    enum class ScreenState {
        Timeline,
        Track,
    };
public:
    Manager(juce::Array<te::AudioTrack*> base_tracks);
    void SetCurrentTrack(size_t track_manager);

    void Render(Interface& interface);
    void HandleEvent(const Event& event);

// private:

    void AddTrack();

    // TODO use array instead
    size_t current_track_;
    ScreenState screen_state_;
    juce::Array<te::AudioTrack*> base_tracks_;
    std::vector<std::unique_ptr<Track>> tracks_;
};

} // namespace box
