#pragma once
#include "util.hh"
#include "track_manager.hh"

namespace box {

class Manager {
public:
    Manager();
    void addTrackManager(size_t index, std::unique_ptr<TrackManager> trackManager);
    void setCurrentTrack(int trackIndex) {
        if (trackIndex >= 0 && trackIndex < NUM_TRACKS) {
            currentTrack = trackIndex;
        }
    }

    void render(Interface& interface);
    void handleEvent(te::Edit& edit, const Event& event);

    // TODO use array instead
    std::vector<std::unique_ptr<TrackManager>> tracks;
    size_t currentTrack;
};

} // namespace box
