#include "manager.hh"

namespace box {

void assert_tracks(const std::vector<std::unique_ptr<TrackManager>> &tracks, size_t index, const std::string &name) {
    if (!(index >= 0 && index < NUM_TRACKS)) {
        throw std::runtime_error{"Manager::" + name + "(): index [" + std::to_string(index) + 
            "] out of range: [0, " + std::to_string(NUM_TRACKS) + "]"};
    }
}

Manager:: Manager() : currentTrack{0} {
    tracks.resize(NUM_TRACKS);
}

void Manager:: addTrackManager(size_t index, std::unique_ptr<TrackManager> trackManager) {
    assert_tracks(tracks, index, "addTrackManager");
    tracks[index] = std::move(trackManager);
}

void Manager:: render(Interface& interface) {
    assert_tracks(tracks, currentTrack, "render");
    tracks[currentTrack]->render(interface);
}

void Manager:: handleEvent(const Event& event) {

    assert_tracks(tracks, currentTrack, "handleEvent");

    if (currentTrack >= 0 && currentTrack < NUM_TRACKS) {
        tracks[currentTrack]->handleEvent(event);
    } else {
        throw std::runtime_error{"Manager::handleEvent(): index [" + std::to_string(currentTrack) + 
            "] out of range [0, " + std::to_string(NUM_TRACKS) + "]"};
    }
}

} // namespace box
