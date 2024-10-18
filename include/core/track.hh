#pragma once

#include <memory>
#include <string>
#include "core/util.hh"
#include "core/interface.hh"
#include "core/plugin.hh"

namespace box {

class Track {
    enum class ScreenState {
        Overview,
        Plugin,
    };
public:
    Track(te::AudioTrack &track);
    ~Track();

    void AddPlugin(std::unique_ptr<Plugin> plugin);
    void SetActivePlugin(int index);
    int GetActivePlugin();
    void RemoveActivePlugin();

    void HandleEvent(const Event& event);
    void Render(Interface& Interface);

    static constexpr int MAX_PLUGINS = 8;
    static constexpr int GRID_SIZE = 4;
    std::vector<std::unique_ptr<Plugin>> plugins_;
    int active_plugin_ = -1;
    te::AudioTrack &track_;
    ScreenState screen_state_ = ScreenState::Overview;
};

} // namespace box
