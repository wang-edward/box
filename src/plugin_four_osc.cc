#include "plugin/four_osc.hh"

namespace box {

FourOscManager:: FourOscManager(te::Plugin *p): 
    PluginManager(p), knob{64, 64, static_cast<te::FourOscPlugin *>(p)->reverbMixValue} {

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

}

} // namespace box
