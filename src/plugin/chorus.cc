#include "plugin/chorus.hh"

#include "core/cv.hh"

namespace box {

Texture2D Chorus::icon_; // Define the static member

Chorus::Chorus(te::Plugin *p)
    : Plugin(p), base_plugin_{static_cast<te::ChorusPlugin *>(p)},
      knob_depth_{Parameter<float>{CV<float>{base_plugin_->depthMs, 0.0, 10.0}},
                  32, // x
                  22, // y
                  16, // radius
                  GREEN,
                  "depth"},
      knob_width_{Parameter<float>{CV<float>{base_plugin_->width, 0.0, 1.0}},
                  96, // x
                  22, // y
                  16, // radius
                  RED,
                  "width"},
      knob_speed_{Parameter<float>{CV<float>{base_plugin_->speedHz, 0.0, 10.0}},
                  32, // x
                  86, // y
                  16, // radius
                  BLUE,
                  "speed"},
      knob_mix_{
          Parameter<float>{CV<float>{base_plugin_->mixProportion, 0.0, 1.0}},
          96, // x
          86, // y
          16, // radius
          YELLOW,
          "mix"} {
    EnsureTextureLoaded();
}

Chorus::~Chorus() { UnloadTexture(icon_); }

Texture2D &Chorus::GetIcon() const { return icon_; }

const char *Chorus::GetIconPath() const { return icon_path_; }

const char *Chorus::GetName() const { return name_; }

void Chorus::Render(Interface &interface) {
    knob_depth_.Render(interface);
    knob_width_.Render(interface);
    knob_speed_.Render(interface);
    knob_mix_.Render(interface);
}

void Chorus::HandleEvent(const Event &event) {
    switch (event.type) {
    case EventType::KeyPress:
        switch (event.value) {
        case KEY_ONE:
            LOG_VAR(knob_depth_.param_.GetNorm());
            knob_depth_.param_.SetNorm(knob_depth_.param_.GetNorm() - 0.125);
            break;
        case KEY_TWO:
            LOG_VAR(knob_depth_.param_.GetNorm());
            knob_depth_.param_.SetNorm(knob_depth_.param_.GetNorm() + 0.125);
            break;
        case KEY_THREE:
            LOG_VAR(knob_depth_.param_.GetNorm());
            knob_width_.param_.SetNorm(knob_width_.param_.GetNorm() - 0.125);
            break;
        case KEY_FOUR:
            LOG_VAR(knob_depth_.param_.GetNorm());
            knob_width_.param_.SetNorm(knob_width_.param_.GetNorm() + 0.125);
            break;
        case KEY_FIVE:
            LOG_VAR(knob_depth_.param_.GetNorm());
            knob_speed_.param_.SetNorm(knob_speed_.param_.GetNorm() - 0.125);
            break;
        case KEY_SIX:
            LOG_VAR(knob_depth_.param_.GetNorm());
            knob_speed_.param_.SetNorm(knob_speed_.param_.GetNorm() + 0.125);
            break;
        case KEY_SEVEN:
            LOG_VAR(knob_depth_.param_.GetNorm());
            knob_mix_.param_.SetNorm(knob_mix_.param_.GetNorm() - 0.125);
            break;
        case KEY_EIGHT:
            LOG_VAR(knob_depth_.param_.GetNorm());
            knob_mix_.param_.SetNorm(knob_mix_.param_.GetNorm() + 0.125);
            break;
        }
        break;
    case EventType::KeyRelease:
        break;
    }
}

} // namespace box
