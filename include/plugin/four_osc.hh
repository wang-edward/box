#pragma once
#include "core/interface.hh"
#include "core/plugin_manager.hh"
#include "component/knob.hh"

namespace box {

class FourOscManager : public PluginManager {
public:
    FourOscManager(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;

private:
    te::FourOscPlugin *base_plugin_;
    Knob<float> knob_;
};


} // namespace box
