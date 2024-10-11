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
    void SetActivePlugin(size_t index);
    size_t GetActivePlugin();

    void HandleEvent(const Event& event);
    void Render(Interface& Interface);

    static constexpr size_t MAX_PLUGINS = 8;
    static constexpr size_t GRID_SIZE = 4;
    std::array<std::unique_ptr<Plugin>, MAX_PLUGINS> plugins_ = { nullptr };
    size_t num_plugins_ = 0;
    size_t active_plugin_ = -1; // TODO what is this bro
    te::AudioTrack &track_;
    ScreenState screen_state_ = ScreenState::Overview;
};

} // namespace box
