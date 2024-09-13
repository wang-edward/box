#include "plugin/delay.hh"

namespace box {

Delay:: Delay(te::Plugin *p): 
    Plugin(p), base_plugin_{static_cast<te::DelayPlugin *>(p)},
    knob_feedback_{32, 64, 5, {0, 255, 0}, base_plugin_->feedbackValue, base_plugin_->feedbackDb},
    knob_mix_{64, 64, 5, {0, 0, 255}, base_plugin_->mixValue, base_plugin_->mixProportion}
{
}

void Delay:: Render(Interface &interface) {
    knob_feedback_.Render(interface);
}

void Delay:: HandleEvent(const Event &event) {
    switch (event.type) {
        case EventType::KeyPress:
            switch(event.value) {
                case GLFW_KEY_1:
                    LOG_VAR(knob_feedback_.param_.GetNorm());
                    knob_feedback_.param_.SetNorm(knob_feedback_.param_.GetNorm() + 0.125);
                    break;
                case GLFW_KEY_2:
                    LOG_VAR(knob_feedback_.param_.GetNorm());
                    knob_feedback_.param_.SetNorm(knob_feedback_.param_.GetNorm() - 0.125);
                    break;
                case GLFW_KEY_3:
                    LOG_VAR(knob_mix_.param_.GetNorm());
                    knob_mix_.param_.SetNorm(knob_mix_.param_.GetNorm() + 0.125);
                    break;
                case GLFW_KEY_4:
                    LOG_VAR(knob_mix_.param_.GetNorm());
                    knob_mix_.param_.SetNorm(knob_mix_.param_.GetNorm() - 0.125);
                    break;
            }
            break;
        case EventType::KeyRelease:
            break;
    }
}

} // namespace box
