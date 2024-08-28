#include "plugin/delay.hh"

namespace box {

Delay:: Delay(te::Plugin *p): 
    PluginManager(p), base_plugin_{static_cast<te::DelayPlugin *>(p)}{
    }

void Delay:: Render(Interface &interface) {
    (void) interface;    
}

void Delay:: HandleEvent(const Event &event) {
    switch (event.type) {
        case EventType::KeyPress:
            switch(event.value) {
                case GLFW_KEY_DOWN:
                    break;
                case GLFW_KEY_UP:
                    break;
            }
            break;
        case EventType::KeyRelease:
            break;
    }
}

} // namespace box
