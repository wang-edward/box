#pragma once
#include "core/util.hh"
#include "core/interface.hh"

namespace box {

class Manager;

struct PluginSelector {
    PluginSelector(std::function<void(const std::string &)> callback);
    void Render(Interface &interface);
    void HandleEvent(const Event &event);
    void Select(const std::string &name);

    const std::vector<std::string> plugin_names_ = {
        te::FourOscPlugin::xmlTypeName,
        te::ChorusPlugin::xmlTypeName,
        te::CompressorPlugin::xmlTypeName,
        te::DelayPlugin::xmlTypeName,
        te::EqualiserPlugin::xmlTypeName,
        te::PhaserPlugin::xmlTypeName,
        te::ReverbPlugin::xmlTypeName,
    };
    size_t current_index_ = 0;
    std::function<void(const std::string &)> callback_;
};


} // namespace box
