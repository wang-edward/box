#include "core/track.hh"
#include "core/util.hh"

namespace box {

static void assert_plugins(const std::vector<std::unique_ptr<Plugin>> &plugins, size_t index, std::string function_name) {
    if (index >= plugins.size()) {
        throw std::runtime_error{"Track::" + function_name + " => index ["+ std::to_string(index) + "] out of range: [0, " + std::to_string(plugins.size()) + "]"};
    }
}

Track:: Track(te::AudioTrack &track): track_{track} {}

void Track:: AddPlugin(std::unique_ptr<Plugin> plugin) {
    size_t index = plugins_.size();
    track_.pluginList.insertPlugin(plugin->GetPlugin(), index, nullptr);
    plugins_.push_back(std::move(plugin));
}

void Track:: SetActivePlugin(size_t index) {
    active_plugin_ = index;
}

size_t Track:: GetActivePlugin() {
    return active_plugin_;
}

void Track:: HandleEvent(const Event& event) {
    assert_plugins(plugins_, active_plugin_, "handleEvent");

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
    plugins_[active_plugin_]->HandleEvent(event);
}

void Track:: Render(Interface& interface) {
    assert_plugins(plugins_, active_plugin_, "render");
    plugins_[active_plugin_]->Render(interface);
}

} // namespace box
