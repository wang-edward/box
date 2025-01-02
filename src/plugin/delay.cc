#include "plugin/delay.hh"

#include "core/cv.hh"

namespace box {

Texture2D Delay::icon_; // Define the static member

Delay::Delay(te::Plugin *p)
    : Plugin(p), base_plugin_{static_cast<te::DelayPlugin *>(p)},
      knob_feedback_{Parameter<float>{base_plugin_->feedbackDb},
                     32, // x
                     22, // y
                     16, // radius
                     GREEN,
                     "feedback"},
      knob_mix_{Parameter<float>{base_plugin_->mixProportion},
                96, // x
                22, // y
                16, // radius
                RED,
                "mix"},
      knob_length_{Parameter<int>{CV<int>{base_plugin_->lengthMs, 0, 10}},
                   32, // x
                   86, // y
                   16, // radius
                   BLUE,
                   "speed"} {
    EnsureTextureLoaded();
}

Delay::~Delay() { UnloadTexture(icon_); }

Texture2D &Delay::GetIcon() const { return icon_; }

const char *Delay::GetIconPath() const { return icon_path_; }

const char *Delay::GetName() const { return name_; }

void Delay::Render(Interface &interface) {
    knob_feedback_.Render(interface);
    knob_mix_.Render(interface);
    knob_length_.Render(interface);
}

void Delay::HandleEvent(const Event &event) {
    switch (event.type) {
    case EventType::KeyPress:
        switch (event.value) {
        case KEY_ONE:
            LOG_VAR(knob_feedback_.param_.GetNorm());
            knob_feedback_.param_.SetNorm(knob_feedback_.param_.GetNorm() -
                                          0.125);
            break;
        case KEY_TWO:
            LOG_VAR(knob_feedback_.param_.GetNorm());
            knob_feedback_.param_.SetNorm(knob_feedback_.param_.GetNorm() +
                                          0.125);
            break;
        case KEY_THREE:
            LOG_VAR(knob_mix_.param_.GetNorm());
            knob_mix_.param_.SetNorm(knob_mix_.param_.GetNorm() - 0.125);
            break;
        case KEY_FOUR:
            LOG_VAR(knob_mix_.param_.GetNorm());
            knob_mix_.param_.SetNorm(knob_mix_.param_.GetNorm() + 0.125);
            break;
        case KEY_FIVE:
            LOG_VAR(knob_length_.param_.GetNorm());
            knob_length_.param_.SetNorm(knob_length_.param_.GetNorm() - 0.125);
            break;
        case KEY_SIX:
            LOG_VAR(knob_length_.param_.GetNorm());
            knob_length_.param_.SetNorm(knob_length_.param_.GetNorm() + 0.125);
            break;
        }
        break;
    case EventType::KeyRelease:
        break;
    }
}

} // namespace box
