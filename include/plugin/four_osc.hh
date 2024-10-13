#pragma once
#include "core/interface.hh"
#include "core/plugin.hh"
#include "component/knob.hh"

namespace box {


class FourOsc : public Plugin {
public:
    FourOsc(te::Plugin *p);
    ~FourOsc() override;

    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;
    Texture2D GetIcon() const override;

private:
    Texture2D icon_;
    te::FourOscPlugin *base_plugin_;
    Knob<float> knob_master_level_;
};


} // namespace box
