#include "plugin/compressor.hh"
#include "core/cv.hh"

namespace box {

Texture2D Compressor::icon_;  // Define the static member

Compressor::Compressor(te::Plugin *p)
    : Plugin(p), base_plugin_{static_cast<te::CompressorPlugin *>(p)},
    knob_threshold_{
        Parameter<float>{base_plugin_->thresholdGain.parameter},
        32, // x
        22, // y
        16, // radius
        GREEN,
        "threshold"
    },
    knob_ratio_{
        Parameter<float>{base_plugin_->ratio.parameter},
        96, // x
        22, // y
        16, // radius
        RED,
        "ratio"
    },
    knob_attack_{
        Parameter<float>{base_plugin_->attackMs.parameter},
        32, // x
        86, // y
        16, // radius
        BLUE,
        "attack"
    },
    knob_release_{
        Parameter<float>{base_plugin_->releaseMs.parameter},
        96, // x
        86, // y
        16, // radius
        YELLOW,
        "release"
    }
{
    EnsureTextureLoaded();
}

Compressor:: ~Compressor()
{
    UnloadTexture(icon_);
}

Texture2D &Compressor:: GetIcon() const
{
    return icon_;
}

const char *Compressor:: GetIconPath() const
{
    return icon_path_;
}


void Compressor::Render(Interface &interface)
{
    knob_threshold_.Render(interface);
    knob_ratio_.Render(interface);
    knob_attack_.Render(interface);
    knob_release_.Render(interface);
}

void Compressor::HandleEvent(const Event &event)
{
    switch (event.type)
    {
    case EventType::KeyPress:
        switch(event.value)
        {
        case KEY_ONE:
            LOG_VAR(knob_threshold_.param_.GetNorm());
            knob_threshold_.param_.SetNorm(knob_threshold_.param_.GetNorm() - 0.125);
            break;
        case KEY_TWO:
            LOG_VAR(knob_threshold_.param_.GetNorm());
            knob_threshold_.param_.SetNorm(knob_threshold_.param_.GetNorm() + 0.125);
            break;
        case KEY_THREE:
            LOG_VAR(knob_ratio_.param_.GetNorm());
            knob_ratio_.param_.SetNorm(knob_ratio_.param_.GetNorm() - 0.125);
            break;
        case KEY_FOUR:
            LOG_VAR(knob_ratio_.param_.GetNorm());
            knob_ratio_.param_.SetNorm(knob_ratio_.param_.GetNorm() + 0.125);
            break;
        case KEY_FIVE:
            LOG_VAR(knob_attack_.param_.GetNorm());
            knob_attack_.param_.SetNorm(knob_attack_.param_.GetNorm() - 0.125);
            break;
        case KEY_SIX:
            LOG_VAR(knob_attack_.param_.GetNorm());
            knob_attack_.param_.SetNorm(knob_attack_.param_.GetNorm() + 0.125);
            break;
        case KEY_SEVEN:
            LOG_VAR(knob_release_.param_.GetNorm());
            knob_release_.param_.SetNorm(knob_release_.param_.GetNorm() - 0.125);
            break;
        case KEY_EIGHT:
            LOG_VAR(knob_release_.param_.GetNorm());
            knob_release_.param_.SetNorm(knob_release_.param_.GetNorm() + 0.125);
            break;
        }
        break;
    case EventType::KeyRelease:
        break;
    }
}

} // namespace box
