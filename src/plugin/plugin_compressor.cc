#include "plugin/compressor.hh"

namespace box {

Compressor::Compressor(te::Plugin *p): 
    PluginManager(p), base_plugin_{static_cast<te::CompressorPlugin *>(p)}{
    }

void Compressor::Render(Interface &interface) {
    
}

void Compressor::HandleEvent(const Event &event) {
    log_msg(LogLevel::Debug, "hello");
    switch (event.type) {
        case EventType::KeyPress:
            switch(event.value) {
                case GLFW_KEY_DOWN:
                    // LOG_VAR(knob_.param_.GetNorm());
                    // knob_.param_.SetNorm(knob_.param_.GetNorm() - 0.125);
                    break;
                case GLFW_KEY_UP:
                    // LOG_VAR(knob_.param_.GetNorm());
                    // knob_.param_.SetNorm(knob_.param_.GetNorm() + 0.125);
                    break;
            }
            break;
        case EventType::KeyRelease:
            break;
    }
}

} // namespace box
