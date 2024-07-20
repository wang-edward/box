#pragma once
#include "util.hh"
#include "track.hh"

namespace box {

class Manager {
public:
    Manager();
    void AddTrackManager(size_t index, std::unique_ptr<Track> Track);
    void SetCurrentTrack(size_t trackIndex);

    void Render(Interface& interface);
    void HandleEvent(const Event& event);

    // TODO use array instead
    std::vector<std::unique_ptr<Track>> tracks_;
    size_t current_track_;
};

} // namespace box
