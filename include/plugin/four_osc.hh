#pragma once
#include "core/interface.hh"
#include "core/plugin.hh"
#include "component/knob.hh"

namespace box {


struct FourOsc : public Plugin {
    static Texture2D icon_;
    static constexpr const char *icon_path_ = "assets/four_16x16.png";

    te::FourOscPlugin *base_plugin_;
    Knob<float> knob_master_level_;

public:

    FourOsc(te::Plugin *p);
    ~FourOsc() override;

    Texture2D &GetIcon() const override;
    const char *GetIconPath() const override;
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;
};


} // namespace box
