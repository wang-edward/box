#include "core/track.hh"
#include "core/util.hh"

namespace box {

static void assert_plugins(const std::vector<std::unique_ptr<Plugin>> &plugins, size_t index, std::string function_name) {
    if (index >= plugins.size()) {
        throw std::runtime_error{"Track::" + function_name + " => index ["+ std::to_string(index) + "] out of range: [0, " + std::to_string(plugins.size()) + "]"};
    }
}

Track:: Track(te::AudioTrack &track): track_{track} {
    Image star = LoadImage("assets/star_16x16_even.png");
    icons_.star = LoadTextureFromImage(star);
    UnloadImage(star);

    Image four = LoadImage("assets/four_16x16.png");
    icons_.four = LoadTextureFromImage(four);
    UnloadImage(four);
}

Track:: ~Track() {
    UnloadTexture(icons_.star);
    UnloadTexture(icons_.four);
}

void Track:: AddPlugin(std::unique_ptr<Plugin> plugin) {
    size_t index = plugins_.size();
    track_.pluginList.insertPlugin(plugin->GetPlugin(), index, nullptr);
    plugins_[num_plugins_] = std::move(plugin);
    num_plugins_ += 1;
}

void Track:: SetActivePlugin(size_t index) {
    active_plugin_ = index;
}

size_t Track:: GetActivePlugin() {
    return active_plugin_;
}

void Track:: HandleEvent(const Event& event) {

    if (event.type == EventType::KeyPress && 
        KEY_TO_MIDI.find(event.value) != KEY_TO_MIDI.end()) {
        auto message = juce::MidiMessage::noteOn (1, KEY_TO_MIDI.at(event.value), (float) 1.0);
        track_.injectLiveMidiMessage(message, 0);
        return;
    } else if (event.type == EventType::KeyRelease &&
        KEY_TO_MIDI.find(event.value) != KEY_TO_MIDI.end()) {
        auto message = juce::MidiMessage::noteOff(1, KEY_TO_MIDI.at(event.value));
        track_.injectLiveMidiMessage(message, 0);
        return;
    }
    switch (screen_state_) {
        case ScreenState::Overview:
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
            for (size_t i = 1; i < GRID_SIZE; i++) {
                float pos = static_cast<float>(i);
                DrawLineV(Vector2{0, pos * 32}, Vector2{128, pos * 32}, WHITE);
                DrawLineV(Vector2{pos * 32, 0}, Vector2{pos * 32, 128}, WHITE);
            }

            for (size_t i = 0; i < MAX_PLUGINS; i++) {
                auto x = static_cast<float>((i % 4) * 32 + 16);
                auto y = static_cast<float>((i / 4) * 32 + 64 + 16);
                if (plugins_[i] == nullptr) {
                    DrawCircleV(Vector2{static_cast<float>(i % 4) * 32 + 16, static_cast<float>(i / 4) * 32 + 64 + 16}, 1.0f, RED);
                    DrawTextPro(GetFontDefault(), "none", Vector2{x, y}, Vector2{11, -4,}, 0.0f, 10.0f, 1.0f, WHITE);
                } else {
                    DrawTexture(plugins_[i]->icon_, x - 8, y - 8, WHITE);
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
