#pragma once
#include "core/util.hh"
#include "core/track.hh"
#include "core/plugin_selector.hh"
#include "core/timeline.hh"
#include "plugin/chorus.hh"
#include "plugin/four_osc.hh"

namespace box {

struct App {
    enum class ScreenState {
        Timeline,
        Track,
        PluginSelector,
    };

    enum class Mode {
        Normal,
        Insert,
    };
    App(te::Edit &edit);
    void Render(Interface& interface);
    void HandleEvent(const Event& event);

    void SetCurrentTrack(size_t track_app);
    void AddTrack();

    ScreenState screen_state_;
    Mode mode_;

    te::Edit &edit_ ;

    size_t current_track_;
    std::vector<std::unique_ptr<Track>> tracks_;
    juce::Array<te::AudioTrack*> base_tracks_;

    PluginSelector plugin_sel_;
    Timeline timeline_;
     
    const std::vector<Color> colors_ = {
        Color{0x29, 0x73, 0x55, 0xff}, 
        Color{0x1e, 0x4d, 0x3f, 0xff},
        Color{0xdd, 0xe6, 0x63, 0xff},
        Color{0x7d, 0x54, 0x39, 0xff},
    };
};

} // namespace box
