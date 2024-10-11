#pragma once
#include "core/interface.hh"
#include "core/plugin.hh"
#include "component/knob.hh"

namespace box {


class Chorus : public Plugin {
public:
    Chorus(te::Plugin *p);
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;
    Texture2D GetIcon() const override;

private:
    Texture2D icon_;
    te::ChorusPlugin *base_plugin_;
    Knob<float> knob_depth_;
};


} // namespace box
