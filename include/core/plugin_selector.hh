#pragma once
#include "core/util.hh"
#include "core/interface.hh"

namespace box {


struct PluginSelector {
    void Render(Interface &interface);
    void HandleEvent(const Event &event);

    const std::vector<std::string> plugin_names_ = {
        "4osc",
        "chorus",
        "compressor",
        "delay",
        "eq",
        "lowpass",
        "phaser",
        "pitchshift",
        "reverb",
    };
    size_t current_index_ = 0;
};


} // namespace box
