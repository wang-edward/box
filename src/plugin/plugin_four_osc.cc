#include "plugin/four_osc.hh"

namespace box {

FourOsc:: FourOsc(te::Plugin *p): 
    PluginManager(p), base_plugin_{static_cast<te::FourOscPlugin *>(p)},
    knob_{0, 0, base_plugin_->masterLevelValue, base_plugin_->masterLevel} {
    }

void FourOsc:: Render(Interface &interface) {
    for (int y = 0; y < Interface::HEIGHT; ++y) {
        for (int x = 0; x < Interface::WIDTH; ++x) {
            // interface.DrawPixel(x, y, {255, 0, 0});
        }
    }
    knob_.Render(interface);
}

void FourOsc:: HandleEvent(const Event &event) {
    log_msg(LogLevel::Debug, "hello");
    switch (event.type) {
        case EventType::KeyPress:
            switch(event.value) {
                case GLFW_KEY_DOWN:
                    LOG_VAR(knob_.param_.GetNorm());
                    knob_.param_.SetNorm(knob_.param_.GetNorm() - 0.125);
                    break;
                case GLFW_KEY_UP:
                    LOG_VAR(knob_.param_.GetNorm());
                    knob_.param_.SetNorm(knob_.param_.GetNorm() + 0.125);
                    break;
            }
            break;
        case EventType::KeyRelease:
            break;
    }
}

} // namespace box
