#include "core/track.hh"
#include "core/util.hh"
#include "core/app.hh"

namespace box {

static void assert_plugins(const std::vector<std::unique_ptr<Plugin>> &plugins, int index, std::string function_name) {
    if (index >= plugins.size()) {
        throw std::runtime_error{"Track::" + function_name + " => index ["+ std::to_string(index) + "] out of range: [0, " + std::to_string(plugins.size()) + "]"};
    }
}

Track:: Track(te::AudioTrack &track): track_{track} 
{
}

Track:: ~Track() 
{
}

void Track:: AddPlugin(std::unique_ptr<Plugin> plugin) 
{
    int index = plugins_.size();
    track_.pluginList.insertPlugin(plugin->GetPlugin(), index, nullptr);
    plugins_[num_plugins_] = std::move(plugin);
    num_plugins_ += 1;
}

void Track:: SetActivePlugin(int index) {
    active_plugin_ = index;
}

int Track:: GetActivePlugin() {
    return active_plugin_;
}

void Track:: HandleEvent(const Event& event) {
    switch (screen_state_) {
    case ScreenState::Overview:
        switch (APP->mode_) {
        case App::Mode::Normal:
            switch (event.type) {
            case EventType::KeyPress:
                switch (event.value) {
                case KEY_ESCAPE:
                    APP->screen_state_ = App::ScreenState::Timeline;
                case KEY_A:
                    LOG_MSG("yo");
                    APP->screen_state_ = App::ScreenState::PluginSelector;
                    break;
                }
                break;
            }
            break;
        case App::Mode::Insert:
            switch (event.type) {
            case EventType::KeyPress:
                if (KEY_TO_MIDI.find(event.value) != KEY_TO_MIDI.end()) {
                    auto message = juce::MidiMessage::noteOn (1, KEY_TO_MIDI.at(event.value), (float) 1.0);
                    track_.injectLiveMidiMessage(message, 0);
                } 
                break;
            case EventType::KeyRelease:
                if (KEY_TO_MIDI.find(event.value) != KEY_TO_MIDI.end()) {
                    auto message = juce::MidiMessage::noteOff(1, KEY_TO_MIDI.at(event.value));
                    track_.injectLiveMidiMessage(message, 0);
                }
                break;
            }
        }
        break;
    case ScreenState::Plugin:
        plugins_[active_plugin_]->HandleEvent(event);
        break;
    }
}

void Track:: Render(Interface& interface) {
    switch (screen_state_) {
        case ScreenState::Overview:
            // draw grid
            for (int i = 1; i < GRID_SIZE; i++) {
                float pos = static_cast<float>(i);
                DrawLineV(Vector2{0, pos * 32}, Vector2{128, pos * 32}, WHITE);
                DrawLineV(Vector2{pos * 32, 0}, Vector2{pos * 32, 128}, WHITE);
            }

            for (int i = 0; i < MAX_PLUGINS; i++) {
                auto x = static_cast<float>((i % 4) * 32 + 16);
                auto y = static_cast<float>((i / 4) * 32 + 64 + 16);
                if (plugins_[i] == nullptr) {
                    DrawCircleV(Vector2{static_cast<float>(i % 4) * 32 + 16, static_cast<float>(i / 4) * 32 + 64 + 16}, 1.0f, RED);
                    DrawTextPro(GetFontDefault(), "none", Vector2{x, y}, Vector2{11, -4,}, 0.0f, 10.0f, 1.0f, WHITE);
                } else {
                    DrawTexture(plugins_[i]->GetIcon(), x - 8, y - 8, WHITE);
                    DrawTextPro(GetFontDefault(), "plugin", Vector2{x, y}, Vector2{11, -4,}, 0.0f, 10.0f, 1.0f, WHITE);
                }
            }
            break;
        case ScreenState::Plugin:
            plugins_[active_plugin_]->Render(interface);
            break;
    }
}

} // namespace box
