#include "plugin/tone_generator.hh"

namespace box {

ToneGenerator::ToneGenerator(te::Plugin *p): 
    Plugin(p), base_plugin_{static_cast<te::ToneGeneratorPlugin *>(p)}{
    }

void ToneGenerator::Render(Interface &interface) {
    (void) interface;
}

void ToneGenerator::HandleEvent(const Event &event) {
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
