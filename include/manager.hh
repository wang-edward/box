#pragma once
#include "util.hh"
#include "track_manager.hh"

namespace box {

class Manager {
public:
    Manager();
    void AddTrackManager(size_t index, std::unique_ptr<TrackManager> track_manager);
    void SetCurrentTrack(size_t track_manager);

    void Render(Interface& interface);
    void HandleEvent(const Event& event);

    // TODO use array instead
    std::vector<std::unique_ptr<TrackManager>> tracks_;
    size_t current_track_;
};

} // namespace box
