#pragma once
#include "core/interface.hh"
#include "core/plugin_manager.hh"

namespace box {


class Equaliser : public PluginManager {
public:
    Equaliser(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;

private:
    te::EqualiserPlugin *base_plugin_;
};


} // namespace box
