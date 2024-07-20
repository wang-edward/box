#pragma once
#include "util.hh"
#include "interface.hh"
#include "plugin_manager.hh"
#include <map>
#include <memory>
#include <string>

namespace box {

class TrackManager {
public:
    TrackManager(te::AudioTrack &track);

    void AddPlugin(std::unique_ptr<PluginManager> plugin);
    void SetActivePlugin(size_t index);
    size_t GetActivePlugin();

    void HandleEvent(const Event& event);
    void Render(Interface& Interface);

private:
    std::vector<std::unique_ptr<PluginManager>> plugins_;
    size_t active_plugin_;
    const te::AudioTrack &track_;
};

} // namespace box
