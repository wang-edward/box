#include "plugin/phaser.hh"

namespace box {

Phaser::Phaser(te::Plugin *p): 
    Plugin(p), base_plugin_{static_cast<te::PhaserPlugin *>(p)}{
    }

void Phaser::Render(Interface &interface) {
    (void) interface;    
}

void Phaser::HandleEvent(const Event &event) {
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
