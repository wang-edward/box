#pragma once

#include <memory>
#include <string>

#include "core/interface.hh"
#include "core/plugin.hh"
#include "core/util.hh"

namespace box
{

class Track
{
    enum class ScreenState
    {
        Overview,
        Plugin,
    };

  public:
    static constexpr int MAX_PLUGINS = 8;
    static constexpr int GRID_SIZE = 4;
    std::vector<std::unique_ptr<Plugin>> plugins_;
    int active_plugin_ = -1;
    te::AudioTrack &base_;
    ScreenState screen_state_ = ScreenState::Overview;

    Track(te::AudioTrack &base);
    ~Track();

    void AddPlugin(std::unique_ptr<Plugin> plugin);
    void SetActivePlugin(int index);
    int GetActivePlugin();
    void RemoveActivePlugin();

    void HandleEvent(const Event &event);
    void Render(Interface &Interface);
};

} // namespace box
