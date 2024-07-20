#include "track_manager.hh"

#include "util.hh"

namespace box {

static void assert_plugins(const std::vector<std::unique_ptr<PluginManager>> &plugins, size_t index, std::string function_name) {
    if (index >= plugins.size()) {
        throw std::runtime_error{"TrackManager::" + function_name + " => index ["+ std::to_string(index) + "] out of range: [0, " + std::to_string(plugins.size()) + "]"};
    }
}

TrackManager:: TrackManager(te::AudioTrack &track): track_{track} {}

void TrackManager:: AddPlugin(std::unique_ptr<PluginManager> plugin) {
    track_.pluginList.insertPlugin(plugin->GetPlugin(), 0, nullptr);
    plugins_.push_back(std::move(plugin));
}

void TrackManager:: SetActivePlugin(size_t index) {
    active_plugin_ = index;
}

size_t TrackManager:: GetActivePlugin() {
    return active_plugin_;
}

void TrackManager:: HandleEvent(const Event& event) {
    assert_plugins(plugins_, active_plugin_, "handleEvent");

    if (event.type == EventType::KeyPress && 
        KEY_TO_MIDI.find(event.value) != KEY_TO_MIDI.end()) {
        auto message = juce::MidiMessage::noteOn (0, KEY_TO_MIDI.at(event.value), (float) 100);
        track_.injectLiveMidiMessage(message, 0);
    } else if (event.type == EventType::KeyRelease &&
        KEY_TO_MIDI.find(event.value) != KEY_TO_MIDI.end()) {
        auto message = juce::MidiMessage::noteOff(0, KEY_TO_MIDI.at(event.value));
        track_.injectLiveMidiMessage(message, 0);
    }
    plugins_[active_plugin_]->HandleEvent(event);
}

void TrackManager:: Render(Interface& interface) {
    assert_plugins(plugins_, active_plugin_, "render");
    plugins_[active_plugin_]->Render(interface);
}

} // namespace box
