#pragma once
#include "util.hh"
#include "track_manager.hh"

namespace box {

class Manager {
public:
    void addTrackManager(size_t index, TrackManager trackManager);
    void setCurrentTrack(int trackIndex) {
        if (trackIndex >= 0 && trackIndex < NUM_TRACKS) {
            currentTrack = trackIndex;
        }
    }

    void render(Interface& interface);
    void handleEvent(te::Edit& edit, const Event& event);

    std::array<TrackManager, NUM_TRACKS> tracks;
    size_t currentTrack;
};

} // namespace box
