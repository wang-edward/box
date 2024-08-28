#include "plugin/chorus.hh"

namespace box {

Chorus::Chorus(te::Plugin *p): 
    PluginManager(p), base_plugin_{static_cast<te::ChorusPlugin *>(p)}{
    }

void Chorus::Render(Interface &interface) {
    (void) interface;
}

void Chorus::HandleEvent(const Event &event) {
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
