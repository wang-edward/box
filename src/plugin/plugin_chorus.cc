#include "plugin/chorus.hh"
#include "core/cv.hh"

namespace box {

Chorus::Chorus(te::Plugin *p)
    : Plugin(p), base_plugin_{static_cast<te::ChorusPlugin *>(p)},
    knob_depth_{CV<float>{base_plugin_->depthMs, 0.0, 100.0}}
    // knob_depth_{32, 64, 5, {0, 255, 0}, base_plugin_->depthMs, nullptr}
{
    Image icon = LoadImage("assets/star_16x16.png");
    icon_ = LoadTextureFromImage(icon);
    UnloadImage(icon);
}

void Chorus::Render(Interface &interface) {
    knob_depth_.Render(interface);
}

void Chorus::HandleEvent(const Event &event) {
    switch (event.type) {
        case EventType::KeyPress:
            switch(event.value) {
                case KEY_ONE:
                    // LOG_VAR(knob_depth_.param_.GetValue());
                    LOG_VAR(knob_depth_.cv_.GetNorm());
                    knob_depth_.cv_.SetNorm(knob_depth_.cv_.GetNorm() + 0.125);
                    break;
                case KEY_TWO:
                    LOG_VAR(knob_depth_.cv_.GetNorm());
                    knob_depth_.cv_.SetNorm(knob_depth_.cv_.GetNorm() - 0.125);
                    break;
            }
            break;
        case EventType::KeyRelease:
            break;
    }
}

Texture2D Chorus:: GetIcon() const {
    return icon_;
}

} // namespace box
