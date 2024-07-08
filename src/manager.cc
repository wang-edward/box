#include "manager.hh"

namespace box {

Manager:: Manager() {}

void Manager:: addTrackManager(size_t index, std::unique_ptr<TrackManager> trackManager) {
    if (index >= 0 && index < NUM_TRACKS) {
        tracks[index] = std::move(trackManager);
    } else {
        throw std::runtime_error{"index [" + std::to_string(index) + "] out of range"};
    }
}

void Manager:: render(Interface& interface) {
    if (currentTrack >= 0 && currentTrack < NUM_TRACKS) {
        tracks[currentTrack]->render(interface);
    }
}

void Manager:: handleEvent(te::Edit& edit, const Event& event) {
    if (currentTrack >= 0 && currentTrack < NUM_TRACKS) {
        tracks[currentTrack]->handleEvent(edit, event);
    }
}

} // namespace box
