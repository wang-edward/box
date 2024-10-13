#include "core/plugin_selector.hh"
#include "core/app.hh"

namespace box {

const std::vector<std::string> PluginSelector:: PLUGIN_NAMES = {
    te::FourOscPlugin::xmlTypeName,
    te::ChorusPlugin::xmlTypeName,
    te::CompressorPlugin::xmlTypeName,
    te::DelayPlugin::xmlTypeName,
    te::EqualiserPlugin::xmlTypeName,
    te::PhaserPlugin::xmlTypeName,
    te::ReverbPlugin::xmlTypeName,
};

void assert_index(std::vector<std::string> v, size_t curr) 
{
    if (curr >= v.size()) throw std::runtime_error{"PluginSelector index out of range: " + std::to_string(curr)};
}

PluginSelector:: PluginSelector()
{

}

void PluginSelector:: Render(Interface &interface) 
{
    for (size_t i = 0; i < interface.HEIGHT / 8; i++) {
        if (i >= PLUGIN_NAMES.size()) break;
        auto name = PLUGIN_NAMES[i];
        float x =  0;
        float y = 16 * i;
        float width = 128;
        float height = 15;
        Color color = RED;
        if (current_index_ == i) {
            color = BLUE;
        }
        DrawRectangleRec(Rectangle{x, y, width, height}, DARKGRAY);
        DrawText(name.c_str(), x, y, 5, color);
    }
}

void PluginSelector:: HandleEvent(const Event &event) 
{
    switch(event.type) {
        case EventType::KeyPress:
            switch (event.value) {
                case KEY_K:
                    current_index_ = clamp_decrement(current_index_);
                    break;
                case KEY_J:
                    current_index_ = std::min(current_index_ + 1, PLUGIN_NAMES.size() - 1);
                    break;
               case KEY_ENTER:
                    APP->screen_state_ = App::ScreenState::Timeline;
                    const auto &name = PLUGIN_NAMES[current_index_];

                    std::unique_ptr<Plugin> p;
                    auto base = APP->edit_.getPluginCache().createNewPlugin(name.c_str(), {}).get();
                    // TODO use cast?
                    if (name == te::ChorusPlugin::xmlTypeName) {
                        p = std::make_unique<Chorus>(base);
                    } else if (name == te::FourOscPlugin::xmlTypeName) {
                        p = std::make_unique<FourOsc>(base);
                    }
                    APP->tracks_[APP->current_track_]-> AddPlugin(std::move(p));
                    break;
            }
            break;
    }
}

} // namespace box
