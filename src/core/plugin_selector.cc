#include "core/plugin_selector.hh"

namespace box {

void assert_index(std::vector<std::string> v, size_t curr) {
    if (curr >= v.size()) throw std::runtime_error{"PluginSelector index out of range: " + std::to_string(curr)};
}

PluginSelector:: PluginSelector(std::function<void(const std::string &)> callback)
    : callback_{callback}
{

}

void PluginSelector:: Render(Interface &interface) {
    for (size_t i = 0; i < interface.HEIGHT / 8; i++) {
        if (i >= plugin_names_.size()) break;
        auto name = plugin_names_[i];
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

void PluginSelector:: HandleEvent(const Event &event) {
    switch(event.type) {
        case EventType::KeyPress:
            switch (event.value) {
                case KEY_K:
                    current_index_ = clamp_decrement(current_index_);
                    break;
                case KEY_J:
                    current_index_ = std::min(current_index_ + 1, plugin_names_.size() - 1);
                    break;
                case KEY_ENTER:
                    callback_(plugin_names_[current_index_]);
            }
            break;
    }
}

} // namespace box
