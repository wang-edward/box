#pragma once
#include "core/interface.hh"
#include "core/plugin.hh"
#include "component/knob.hh"

namespace box
{

struct Eq : public Plugin
{
    static Texture2D icon_;
    static constexpr const char *icon_path_ = "assets/star_16x16.png";
    static constexpr const char *name_ = "eq";

    te::EqualiserPlugin *base_plugin_;
    Knob<float> knob_lo_freq_value_;
    Knob<float> knob_lo_gain_value_;
    Knob<float> knob_lo_q_value_;
    Knob<bool> knob_phase_invert_;

public:

    Eq(te::Plugin *p);
    ~Eq() override;

    Texture2D &GetIcon() const override;
    const char *GetIconPath() const override;
    const char *GetName() const override;
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;
};

}