#pragma once
#include "core/interface.hh"
#include "core/plugin_manager.hh"

namespace box {


class Chorus : public PluginManager {
public:
    Chorus(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;

private:
    te::ChorusPlugin *base_plugin_;
};


} // namespace box
