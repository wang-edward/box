#include "plugin/equaliser.hh"

namespace box {

Equaliser::Equaliser(te::Plugin *p): 
    Plugin(p), base_plugin_{static_cast<te::EqualiserPlugin *>(p)}{
    }

void Equaliser::Render(Interface &interface) {
    (void) interface;    
}

void Equaliser::HandleEvent(const Event &event) {
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
