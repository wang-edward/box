#include "plugin/chorus.hh"

namespace box {

Chorus::Chorus(te::Plugin *p): 
    Plugin(p), base_plugin_{static_cast<te::ChorusPlugin *>(p)},
    knob_depth_{32, 64, 5, {0, 255, 0}, base_plugin_->depthMs, nullptr}
{
}

void Chorus::Render(Interface &interface) {
    knob_depth_.Render(interface);
}

void Chorus::HandleEvent(const Event &event) {
    switch (event.type) {
        case EventType::KeyPress:
            switch(event.value) {
                case KEY_ONE:
                    LOG_VAR(knob_depth_.param_.GetValue());
                    knob_depth_.param_.SetValue(knob_depth_.param_.GetValue() + 0.125);
                    break;
                case KEY_TWO:
                    LOG_VAR(knob_depth_.param_.GetValue());
                    knob_depth_.param_.SetValue(knob_depth_.param_.GetValue() - 0.125);
                    break;
            }
            break;
        case EventType::KeyRelease:
            break;
    }
}

} // namespace box
