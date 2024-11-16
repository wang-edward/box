#include "plugin/four_osc.hh"

namespace box {

FourOsc:: FourOsc(te::Plugin *p): 
    Plugin(p), base_plugin_{static_cast<te::FourOscPlugin *>(p)},
    // knob_master_level_{32, 64, 16, {255,0,0}, base_plugin_->masterLevelValue, base_plugin_->masterLevel}

    knob_master_level_{
        Parameter<float>(base_plugin_->masterLevel),
        32, // x
        22, // y
        16, // radius
        GREEN,
        "volume"
    }
{
    Image four = LoadImage("assets/four_16x16.png");
    icon_ = LoadTextureFromImage(four);
    UnloadImage(four);

    // TODO for testing effects
    for (int i = 0; i < base_plugin_->oscParams.size(); i++)
    {
        base_plugin_->oscParams[i]->waveShapeValue.setValue(2, nullptr);
    }
}

FourOsc:: ~FourOsc()
{
    UnloadTexture(icon_);
}

void FourOsc:: Render(Interface &interface)
{
    for (int y = 0; y < Interface::HEIGHT; ++y)
    {
        for (int x = 0; x < Interface::WIDTH; ++x)
        {
            if (x % 2 == 0 && y % 2 == 0)
            {
                DrawPixel(x, y, RED);
            }
        }
    }
    knob_master_level_.Render(interface);
}

void FourOsc:: HandleEvent(const Event &event)
{
    switch (event.type)
    {
    case EventType::KeyPress:
        switch(event.value)
        {
        case KEY_ONE:
            LOG_VAR(knob_master_level_.param_.GetNorm());
            knob_master_level_.param_.SetNorm(knob_master_level_.param_.GetNorm() - 0.125);
            break;
        case KEY_TWO:
            LOG_VAR(knob_master_level_.param_.GetNorm());
            knob_master_level_.param_.SetNorm(knob_master_level_.param_.GetNorm() + 0.125);
            break;
        }
        break;
    case EventType::KeyRelease:
        break;
    }
}

Texture2D FourOsc:: GetIcon() const
{
    return icon_;
}

} // namespace box
