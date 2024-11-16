#pragma once
#include "core/interface.hh"
#include "core/plugin.hh"
#include "component/knob.hh"

namespace box
{

class Compressor : public Plugin
{
public:
    Compressor(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;
    Texture2D GetIcon() const override;

private:
    Texture2D icon_;
    te::CompressorPlugin *base_plugin_;
    Knob<float> knob_threshold_;
    Knob<float> knob_ratio_;
    Knob<float> knob_attack_;
    Knob<float> knob_release_;
};

}
