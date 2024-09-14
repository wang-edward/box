#include "plugin/reverb.hh"

namespace box {

Reverb::Reverb(te::Plugin *p): 
    Plugin(p), base_plugin_{static_cast<te::ReverbPlugin *>(p)}{
    }

void Reverb::Render(Interface &interface) {
    (void) interface;
}

void Reverb::HandleEvent(const Event &event) {
    switch (event.type) {
        case EventType::KeyPress:
            switch(event.value) {
                case KEY_DOWN:
                    break;
                case KEY_UP:
                    break;
            }
            break;
        case EventType::KeyRelease:
            break;
    }
}

} // namespace box
