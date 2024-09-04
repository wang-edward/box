#pragma once
#include "core/interface.hh"
#include "core/plugin_manager.hh"
#include "component/knob.hh"

namespace box {


class Delay : public PluginManager {
public:
    Delay(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;

private:
    te::DelayPlugin *base_plugin_;
    Knob<float> knob_feedback_;
};


} // namespace box
