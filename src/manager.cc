#include "manager.hh"

namespace box {

void assert_tracks(const std::vector<std::unique_ptr<TrackManager>> &tracks, size_t index, const std::string &name) {
    if (!(index >= 0 && index < NUM_TRACKS)) {
        throw std::runtime_error{"Manager::" + name + "(): index [" + std::to_string(index) + 
            "] out of range: [0, " + std::to_string(NUM_TRACKS) + "]"};
    }
}

Manager:: Manager() : current_track_{0} {
    tracks_.resize(NUM_TRACKS);
}

void Manager:: add_track_manager(size_t index, std::unique_ptr<TrackManager> trackManager) {
    assert_tracks(tracks_, index, "addTrackManager");
    tracks_[index] = std::move(trackManager);
}

void Manager:: render(Interface& interface) {
    assert_tracks(tracks_, current_track_, "render");
    tracks_[current_track_]->render(interface);
}

void Manager:: handle_event(const Event& event) {

    assert_tracks(tracks_, current_track_, "handleEvent");

    if (current_track_ >= 0 && current_track_ < NUM_TRACKS) {
        tracks_[current_track_]->handle_event(event);
    } else {
        throw std::runtime_error{"Manager::handleEvent(): index [" + std::to_string(current_track_) + 
            "] out of range [0, " + std::to_string(NUM_TRACKS) + "]"};
    }
}

} // namespace box
