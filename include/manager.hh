#pragma once
#include "util.hh"
#include "track_manager.hh"

namespace box {

class Manager {
public:
    Manager();
    void add_track_manager(size_t index, std::unique_ptr<TrackManager> trackManager);
    void set_current_track(int trackIndex) {
        if (trackIndex >= 0 && trackIndex < NUM_TRACKS) {
            current_track_ = trackIndex;
        }
    }

    void render(Interface& interface);
    void handle_event(const Event& event);

    // TODO use array instead
    std::vector<std::unique_ptr<TrackManager>> tracks_;
    size_t current_track_;
};

} // namespace box
