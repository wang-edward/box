#include "plugin/compressor.hh"

namespace box {

Compressor::Compressor(te::Plugin *p): 
    Plugin(p), base_plugin_{static_cast<te::CompressorPlugin *>(p)}{
    }

void Compressor::Render(Interface &interface) {
    (void) interface;
}

void Compressor::HandleEvent(const Event &event) {
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
