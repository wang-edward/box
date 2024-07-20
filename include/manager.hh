#pragma once
#include "util.hh"
#include "track.hh"

namespace box {

class Manager {
public:
    Manager();
    void add_track_manager(size_t index, std::unique_ptr<Track> Track);
    void set_current_track(size_t trackIndex);

    void render(Interface& interface);
    void handle_event(const Event& event);

    // TODO use array instead
    std::vector<std::unique_ptr<Track>> tracks_;
    size_t current_track_;
};

} // namespace box
