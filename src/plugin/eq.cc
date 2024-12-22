#include "plugin/eq.hh"

namespace box {

Texture2D Chorus::icon_;  // Define the static member

Eq::Eq(te::Plugin *p)
    : Plugin(p), base_plugin_{static_cast<te::ChorusPlugin *>(p)},
    knob_lo_freq_value_{
        Parameter<float>{base_plugin_->loFreq},
        32, // x
        22, // y
        16, // radius
        GREEN,
        "low frequency value"
    },
    knob_lo_gain_value_{
        Parameter<float>{base_plugin_->loGain},
        96, // x
        22, // y
        16, // radius
        RED,
        "low gain value"
    },
    knob_lo_q_value_{
        Parameter<float>{base_plugin_->loQ},
        32, // x
        86, // y
        16, // radius
        BLUE,
        "low q Value"
    },
    knob_phase_invert_{
        Parameter<float>{CV<bool>{base_plugin_->phaseInvert, false}},
        96, // x
        86, // y
        16, // radius
        YELLOW,
        "phase invert"
    }

{
    EnsureTextureLoaded();
}

Eq:: ~Eq()
{
    UnloadTexture(icon_);
}

Texture2D &Eq:: GetIcon() const
{
    return icon_;
}

const char *Eq::GetIconPath() const
{
    return icon_path_;
}

const char *Eq::GetName() const
{
    return name_;
}

void Chorus::Render(Interface &interface)
{
    knob_depth_.Render(interface);
    knob_width_.Render(interface);
    knob_speed_.Render(interface);
    knob_lo_gain_value_.Render(interface);
}

void Chorus::HandleEvent(const Event &event)
{
    // TODO: Figure out key presses with Automated Parameters
    switch (event.type)
    {
    case EventType::KeyPress:
        switch(event.value)
        {
        case KEY_ONE:
            LOG_VAR(knob_lo_freq_value_.param_.GetNorm());
            knob_lo_freq_value_.param_.SetNorm(knob_lo_freq_value_.param_.GetNorm() - 0.125);
            break;
        case KEY_TWO:
            LOG_VAR(knob_lo_freq_value_.param_.GetNorm());
            knob_lo_freq_value_.param_.SetNorm(knob_lo_freq_value_.param_.GetNorm() + 0.125);
            break;
        case KEY_THREE:
            LOG_VAR(knob_lo_gain_value_.param_.GetNorm());
            knob_lo_gain_value_.param_.SetNorm(knob_lo_gain_value_.param_.GetNorm() - 0.125);
            break;
        case KEY_FOUR:
            LOG_VAR(knob_lo_gain_value_.param_.GetNorm());
            knob_lo_gain_value_.param_.SetNorm(knob_lo_gain_value_.param_.GetNorm() + 0.125);
            break;
        case KEY_FIVE:
            LOG_VAR(knob_lo_q_value_.param_.GetNorm());
            knob_lo_q_value_.param_.SetNorm(knob_lo_q_value_.param_.GetNorm() - 0.125);
            break;
        case KEY_SIX:
            LOG_VAR(knob_lo_q_value_.param_.GetNorm());
            knob_lo_q_value_.param_.SetNorm(knob_lo_q_value_.param_.GetNorm() + 0.125);
            break;
        case KEY_SEVEN:
            // TODO: Need to implement boolean support first
            // LOG_VAR(knob_lo_q_value_.param_.GetNorm());
            // knob_lo_q_value_.param_.SetNorm(knob_lo_q_value_.param_.GetNorm() - 0.125);
            break;
        case KEY_EIGHT:
            // TODO: Need to implement boolean support first
            // LOG_VAR(knob_lo_q_value_.param_.GetNorm());
            // knob_lo_q_value_.param_.SetNorm(knob_lo_q_value_.param_.GetNorm() + 0.125);
            break;
        }
        break;
    case EventType::KeyRelease:
        break;
    }
}

} // namespace box
