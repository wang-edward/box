#pragma once
#include "core/interface.hh"
#include "core/plugin_manager.hh"

namespace box {


class Delay : public PluginManager {
public:
    Delay(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;

private:
    te::DelayPlugin *base_plugin_;
};


} // namespace box
