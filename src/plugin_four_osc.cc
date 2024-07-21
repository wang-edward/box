#include "plugin/four_osc.hh"

namespace box {

FourOscManager:: FourOscManager(te::Plugin *p): 
    PluginManager(p), knob{64, 64, static_cast<te::FourOscPlugin *>(p)->masterLevelValue} {
}

void FourOscManager:: Render(Interface &interface) {
    for (int y = 0; y < Interface::HEIGHT; ++y) {
        for (int x = 0; x < Interface::WIDTH; ++x) {
            // interface.DrawPixel(x, y, {255, 0, 0});
        }
    }
    knob.Render(interface);
}

void FourOscManager:: HandleEvent(const Event &event) {
    log_msg(LogLevel::Debug, "hello");
    switch (event.type) {
        case EventType::KeyPress:
            switch(event.value) {
                case GLFW_KEY_UP:
                    log_msg(LogLevel::Off, std::to_string(knob.GetValue()));
                    knob.SetValue(knob.GetValue() * 0.5);
                break;
            }
            break;
        case EventType::KeyRelease:
            break;
    }
}

} // namespace box
