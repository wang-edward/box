#pragma once
#include "component/knob.hh"
#include "core/interface.hh"
#include "core/plugin.hh"

namespace box {

struct Compressor : public Plugin {
  static Texture2D icon_;
  static constexpr const char *icon_path_ = "assets/star_16x16.png";
  static constexpr const char *name_ = "cmpr";

  te::CompressorPlugin *base_plugin_;
  Knob<float> knob_threshold_;
  Knob<float> knob_ratio_;
  Knob<float> knob_attack_;
  Knob<float> knob_release_;

 public:
  Compressor(te::Plugin *p);
  ~Compressor() override;

  Texture2D &GetIcon() const override;
  const char *GetIconPath() const override;
  const char *GetName() const override;
  void Render(Interface &interface) override;
  void HandleEvent(const Event &event) override;
};

}  // namespace box
