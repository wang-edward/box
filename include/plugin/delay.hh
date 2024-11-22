#pragma once
#include "core/interface.hh"
#include "core/plugin.hh"
#include "component/knob.hh"

namespace box
{

struct Delay : public Plugin
{
    static Texture2D icon_;
    static constexpr const char *icon_path_ = "assets/star_16x16.png";
    static constexpr const char *name_ = "dlay";

    te::DelayPlugin *base_plugin_;
    Knob<float> knob_feedback_;
    Knob<float> knob_mix_;
    Knob<int> knob_length_;

public:

    Delay(te::Plugin *p);
    ~Delay() override;

    Texture2D &GetIcon() const override;
    const char *GetIconPath() const override;
    const char *GetName() const override;
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;
};

} // namespace box
