#pragma once
#include "core/util.hh"
#include "core/track.hh"
#include "core/plugin_selector.hh"
#include "core/timeline.hh"

namespace box {

class Manager {
    enum class ScreenState {
        Timeline,
        Track,
        PluginSelector,
    };
public:
    Manager(te::Edit &edit);
    void SetCurrentTrack(size_t track_manager);

    void Render(Interface& interface);
    void HandleEvent(const Event& event);

// private:

    void AddTrack();

    // TODO use array instead
    te::Edit &edit_ ;
    const std::vector<Color> colors_ = {
        Color{0x29, 0x73, 0x55, 0xff}, 
        Color{0x1e, 0x4d, 0x3f, 0xff},
        Color{0xdd, 0xe6, 0x63, 0xff},
        Color{0x7d, 0x54, 0x39, 0xff},
    };
    ScreenState screen_state_;

    size_t current_track_;
    std::vector<std::unique_ptr<Track>> tracks_;
    juce::Array<te::AudioTrack*> base_tracks_;
    PluginSelector plugin_selector_;
    // Timeline timeline_;

    size_t sel_current_index_ = 0;
    const std::vector<std::string> sel_plugin_names_ = {
        te::FourOscPlugin::xmlTypeName,
        te::ChorusPlugin::xmlTypeName,
        te::ReverbPlugin::xmlTypeName,
        te::DelayPlugin::xmlTypeName,
    };
};

} // namespace box
