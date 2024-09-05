#pragma once
#include "core/interface.hh"
#include "core/plugin.hh"
#include "component/knob.hh"

namespace box {


class FourOsc : public Plugin {
public:
    FourOsc(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;

private:
    te::FourOscPlugin *base_plugin_;
    Knob<float> knob_master_level_;
};


} // namespace box
