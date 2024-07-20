#include "core/manager.hh"

namespace box {

static void assert_tracks(const std::vector<std::unique_ptr<TrackManager>> &tracks, size_t index, const std::string &name) {
    if (!(index >= 0 && index < NUM_TRACKS)) {
        throw std::runtime_error{"Manager::" + name + "(): index [" + std::to_string(index) + 
            "] out of range: [0, " + std::to_string(NUM_TRACKS) + "]"};
    }
}

Manager:: Manager() : current_track_{0} {
    tracks_.resize(NUM_TRACKS);
}

void Manager:: AddTrackManager(size_t index, std::unique_ptr<TrackManager> track_manager) {
    assert_tracks(tracks_, index, "addTrack");
    tracks_[index] = std::move(track_manager);
}

void Manager:: SetCurrentTrack(size_t track_index) {
    if (track_index >= 0 && track_index < NUM_TRACKS) {
        current_track_ = track_index;
    }
}

void Manager:: Render(Interface& interface) {
    assert_tracks(tracks_, current_track_, "render");
    tracks_[current_track_]->Render(interface);
}

void Manager:: HandleEvent(const Event& event) {

    assert_tracks(tracks_, current_track_, "handleEvent");

    if (current_track_ >= 0 && current_track_ < NUM_TRACKS) {
        tracks_[current_track_]->HandleEvent(event);
    } else {
        throw std::runtime_error{"Manager::handleEvent(): index [" + std::to_string(current_track_) + 
            "] out of range [0, " + std::to_string(NUM_TRACKS) + "]"};
    }
}

} // namespace box
