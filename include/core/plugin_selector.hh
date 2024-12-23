#pragma once
#include "core/util.hh"
#include "core/interface.hh"

namespace box {

struct PluginSelector {
    PluginSelector();
    void Render(Interface &interface);
    void HandleEvent(const Event &event);
    void Select(const std::string &name);

    static const std::vector<std::string> PLUGIN_NAMES;
    size_t current_index_ = 0;
};

} // namespace box
