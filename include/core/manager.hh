#pragma once
#include "core/util.hh"
#include "core/track.hh"

namespace box {

class Manager {
public:
    Manager();
    void AddTrack(size_t index, std::unique_ptr<Track> track_manager);
    void SetCurrentTrack(size_t track_manager);

    void Render(Interface& interface);
    void HandleEvent(const Event& event);

    // TODO use array instead
    std::vector<std::unique_ptr<Track>> tracks_;
    size_t current_track_;
};

} // namespace box
