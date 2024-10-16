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

    void HandleEvent(const Event& event);
    void Render(Interface& Interface);

    static constexpr int MAX_PLUGINS = 8;
    static constexpr int GRID_SIZE = 4;
    std::array<std::unique_ptr<Plugin>, MAX_PLUGINS> plugins_ = { nullptr };
    int num_plugins_ = 0;
    int active_plugin_ = 0; // TODO what is this bro
    te::AudioTrack &track_;
    ScreenState screen_state_ = ScreenState::Overview;
};

} // namespace box
