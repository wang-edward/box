#pragma once
#include "core/util.hh"
#include "core/track.hh"
#include "core/plugin_selector.hh"
#include "core/timeline.hh"
#include "plugin/chorus.hh"
#include "plugin/four_osc.hh"

namespace box {

class Manager {
public:
    enum class ScreenState {
        Timeline,
        Track,
        PluginSelector,
    };
    Manager(te::Edit &edit);
    void SetCurrentTrack(size_t track_manager);

    void Render(Interface& interface);
    void HandleEvent(const Event& event);

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

    std::function<void(const std::string &)> plugin_sel_callback_ = [this] (const std::string &name) {
        screen_state_ = ScreenState::Timeline;
        std::cout << name << std::endl;
        
        std::unique_ptr<Plugin> p;
        auto base = edit_.getPluginCache().createNewPlugin(name.c_str(), {}).get();

        // TODO use cast?
        if (name == te::ChorusPlugin::xmlTypeName) {
            p = std::make_unique<Chorus>(base);
        } else if (name == te::FourOscPlugin::xmlTypeName) {
            p = std::make_unique<FourOsc>(base);
        }

        tracks_[current_track_]-> AddPlugin(std::move(p));
    };
    PluginSelector plugin_sel_;
    Timeline timeline_;
};

} // namespace box
