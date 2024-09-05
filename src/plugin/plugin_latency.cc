#include "plugin/latency.hh"

namespace box {

Latency::Latency(te::Plugin *p): 
    Plugin(p), base_plugin_{static_cast<te::LatencyPlugin *>(p)}{
    }

void Latency::Render(Interface &interface) {
    (void) interface;
}

void Latency::HandleEvent(const Event &event) {
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
