#pragma once
#include "core/interface.hh"
#include "core/plugin.hh"
#include "component/knob.hh"

namespace box
{


class Delay : public Plugin
{
public:
    Delay(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;
    Texture2D GetIcon() const override;

private:
    Texture2D icon_;
    te::DelayPlugin *base_plugin_;
    Knob<float> knob_feedback_;
    Knob<float> knob_mix_;
    Knob<int> knob_length_;

};


} // namespace box
