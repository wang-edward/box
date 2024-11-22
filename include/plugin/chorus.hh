#pragma once
#include "core/interface.hh"
#include "core/plugin.hh"
#include "component/knob.hh"

namespace box
{


struct Chorus : public Plugin
{
    static Texture2D icon_;
    static constexpr const char *icon_path_ = "assets/star_16x16.png";
    static constexpr const char *name_ = "chorus";

    te::ChorusPlugin *base_plugin_;
    Knob<float> knob_depth_;
    Knob<float> knob_width_;
    Knob<float> knob_speed_;
    Knob<float> knob_mix_;

public:

    Chorus(te::Plugin *p);
    ~Chorus() override; 

    Texture2D &GetIcon() const override;
    const char *GetIconPath() const override;
    const char *GetName() const override;
    void Render(Interface &interface) override;
    void HandleEvent(const Event& event) override;
};

} // namespace box
