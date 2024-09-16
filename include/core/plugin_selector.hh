#pragma once
#include "core/util.hh"
#include "core/plugin.hh"
#include "core/interface.hh"

#include "plugin/four_osc.hh"
#include "plugin/delay.hh"
#include "plugin/phaser.hh"
#include "plugin/chorus.hh"
#include "plugin/compressor.hh"
#include "plugin/equaliser.hh"
#include "plugin/reverb.hh"


namespace box {

template <typename T>
std::unique_ptr<Plugin> create_plugin(te::Plugin *p) {
    return std::make_unique<T>(p);
}


struct PluginSelector {
    PluginSelector(std::function<void(std::string, std::unique_ptr<Plugin>)> on_select_callback);
    void Render(Interface &interface);
    void HandleEvent(const Event &event);

    const std::vector<std::pair<std::string, std::unique_ptr<Plugin>(*)(te::Plugin *)>> plugins_ = {
        {te::FourOscPlugin::xmlTypeName, &create_plugin<FourOsc>},
        {te::ChorusPlugin::xmlTypeName, &create_plugin<Chorus>},
        {te::CompressorPlugin::xmlTypeName, &create_plugin<Compressor>},
        {te::DelayPlugin::xmlTypeName, &create_plugin<Delay>},
        {te::EqualiserPlugin::xmlTypeName, &create_plugin<Equaliser>},
        {te::PhaserPlugin::xmlTypeName, &create_plugin<Phaser>},
        {te::ReverbPlugin::xmlTypeName, &create_plugin<Reverb>},
    };
    std::function<void(std::string, std::unique_ptr<Plugin>)> on_select_callback_;
    size_t current_index_ = 0;
};


} // namespace box
