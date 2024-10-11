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

    static const std::vector<std::string> PLUGIN_NAMES;
    size_t current_index_ = 0;
    std::function<void(const std::string &)> callback_;
};

const std::vector<std::string> PluginSelector:: PLUGIN_NAMES = {
    te::FourOscPlugin::xmlTypeName,
    te::ChorusPlugin::xmlTypeName,
    te::CompressorPlugin::xmlTypeName,
    te::DelayPlugin::xmlTypeName,
    te::EqualiserPlugin::xmlTypeName,
    te::PhaserPlugin::xmlTypeName,
    te::ReverbPlugin::xmlTypeName,
};


} // namespace box
