#include "plugin/four_osc.hh"

namespace box {

FourOsc:: FourOsc(te::Plugin *p): 
    Plugin(p), base_plugin_{static_cast<te::FourOscPlugin *>(p)},
    knob_master_level_{32, 64, 5, {255,0,0}, base_plugin_->masterLevelValue, base_plugin_->masterLevel}
{
}

void FourOsc:: Render(Interface &interface) {
    for (int y = 0; y < Interface::HEIGHT; ++y) {
        for (int x = 0; x < Interface::WIDTH; ++x) {
            if (x % 2 == 0 && y % 2 == 0) {
                DrawCircle(x, y, 3, RED);
            }
        }
    }
    knob_master_level_.Render(interface);
}

void FourOsc:: HandleEvent(const Event &event) {
    log_msg(LogLevel::Debug, "hello");
    switch (event.type) {
        case EventType::KeyPress:
            switch(event.value) {
                case KEY_DOWN:
                    LOG_VAR(knob_master_level_.param_.GetNorm());
                    knob_master_level_.param_.SetNorm(knob_master_level_.param_.GetNorm() - 0.125);
                    break;
                case KEY_UP:
                    LOG_VAR(knob_master_level_.param_.GetNorm());
                    knob_master_level_.param_.SetNorm(knob_master_level_.param_.GetNorm() + 0.125);
                    break;
            }
            break;
        case EventType::KeyRelease:
            break;
    }
}

} // namespace box
