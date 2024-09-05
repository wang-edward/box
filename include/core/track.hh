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

    void AddPlugin(std::unique_ptr<Plugin> plugin);
    void SetActivePlugin(size_t index);
    size_t GetActivePlugin();

    void HandleEvent(const Event& event);
    void Render(Interface& Interface);

private:
    std::vector<std::unique_ptr<Plugin>> plugins_;
    size_t active_plugin_;
    te::AudioTrack &track_;
};

} // namespace box
