#include "core/app.hh"
#include <algorithm>

namespace box {

static void assert_tracks(const std::vector<std::unique_ptr<Track>> &tracks, size_t index, const std::string &name) 
{
    if (tracks.size() == 0 || index >= tracks.size()) 
    {
        throw std::runtime_error{"App::" + name + "(): index [" + std::to_string(index) + 
            "] out of range: [0, " + std::to_string(tracks.size()) + "]"};
    }
}

void print_tracks(App a)
{
    for (auto &t : a.tracks_)
    {
        // std::cout << "Wave Input Device: " << t->track_.getWaveInputDevice() << std::endl;
        // WaveInputDevice& getWaveInputDevice() const noexcept        { jassert (waveInputDevice); return *waveInputDevice; }
        // MidiInputDevice& getMidiInputDevice() const noexcept        { jassert (midiInputDevice); return *midiInputDevice; }
    }
}

App:: App(te::Engine &engine, te::Edit &edit):
    engine_{engine},
    edit_{edit},
    current_track_{0},
    base_tracks_{te::getAudioTracks(edit)}
{
    AddTrack(); // ensure there's always at least 1
    LOG_VAR(tracks_.size());
    ArmMidi(current_track_); // turn on midi for it
}

void App:: AddTrack()
{
    tracks_.push_back(std::make_unique<Track>(*base_tracks_[tracks_.size()]));
}

size_t App:: GetCurrTrack()
{
    return current_track_;
}
void App:: SetCurrTrack(size_t track_index)
{
    size_t old = current_track_;
    if (track_index >= 0 && track_index < MAX_TRACKS) 
    {
        current_track_ = track_index;
    }
    else
    {
        throw std::runtime_error{"SetCurrTrack() out of range: "};
    }

    UnarmMidi(old);
    ArmMidi(current_track_);
}

void App:: ArmMidi(size_t index)
{
    assert(0 <= index && index < tracks_.size());
    for (auto instance : edit_.getAllInputDevices())
    {
        auto device_type = instance->getInputDevice().getDeviceType();
        if (device_type == te::InputDevice::physicalMidiDevice ||
            device_type == te::InputDevice::virtualMidiDevice)
        {
            auto t = te::getAudioTracks(edit_)[index];
            if (t != nullptr)
            {
                [[ maybe_unused ]] auto res = instance->setTarget (t->itemID, true, &edit_.getUndoManager(), 0);
                instance->setRecordingEnabled (t->itemID, true);
            }
        }
    }
}
void App:: UnarmMidi(size_t index)
{
    assert(0 <= index && index < tracks_.size());
    for (auto instance : edit_.getAllInputDevices())
    {
        auto device_type = instance->getInputDevice().getDeviceType();
        if (device_type == te::InputDevice::physicalMidiDevice ||
            device_type == te::InputDevice::virtualMidiDevice)
        {
            auto t = te::getAudioTracks(edit_)[index];
            if (t != nullptr)
            {
                instance->removeTarget(t->itemID, &edit_.getUndoManager());
            }
        }
    }
}

void App:: Render(Interface& interface) 
{
    assert_tracks(tracks_, current_track_, "render");

    std::string text = std::to_string(GetFPS());
    DrawText(text.c_str(), 115, 0, 10, WHITE);

    switch (screen_state_) 
    {
    case ScreenState::Timeline:
        timeline_.Render(interface);
        break;
    case ScreenState::Track:
        tracks_[current_track_]->Render(interface);
        break;
    case ScreenState::PluginSelector:
        plugin_sel_.Render(interface);
        break;
    }

    if (mode_ == Mode::Insert)
    {
        DrawRectangleLines(0, 0, 128, 127, PURPLE); // -1 so bottom row shows
    }
    if (edit_.getTransport().isRecording())
    {
        DrawRectangleLines(1, 1, 126, 125, RED); // -1 so bottom row shows
    }
}

void App:: HandleEvent(const Event& event) 
{
    assert_tracks(tracks_, current_track_, "handleEvent");
    if (event.type == EventType::KeyPress && event.value == KEY_I) 
    {
        if (mode_ == Mode::Normal) 
        {
            mode_ = Mode::Insert;
        } 
        else if (mode_ == Mode::Insert) 
        {
            mode_ = Mode::Normal;
        }
    }

    if (mode_ == Mode::Insert)
    {
        switch (event.type)
        {
        case EventType::KeyPress:
            LOG_VAR(event.value);
            if (event.value == KEY_UP)
            {
                APP->key_offset_ += 1;
                break;
            }
            if (event.value == KEY_DOWN)
            {
                APP->key_offset_ -= 1;
                break;
            }

            if (KEY_TO_MIDI.find(event.value) != KEY_TO_MIDI.end()
                && active_notes_.find(event.value) == active_notes_.end())
            {
                int note = (12 * key_offset_) + KEY_TO_MIDI.at(event.value);

                active_notes_[event.value] = note;
                auto message = juce::MidiMessage::noteOn(1, note, 1.0f);
                // tracks_[current_track_]->base_.injectLiveMidiMessage(message, 0);
                te::MidiInputDevice* dev = engine_.getDeviceManager().getDefaultMidiInDevice();
                dev->keyboardState.noteOn(1, note, 1.0);
            }
            break;
        case EventType::KeyRelease:

            if (KEY_TO_MIDI.find(event.value) != KEY_TO_MIDI.end()
                && active_notes_.find(event.value) != active_notes_.end())
            {
                int note = active_notes_[event.value];

                active_notes_.erase(event.value);
                auto message = juce::MidiMessage::noteOff(1, note);
                // tracks_[current_track_]->base_.injectLiveMidiMessage(message, 0);
                te::MidiInputDevice* dev = engine_.getDeviceManager().getDefaultMidiInDevice();
                dev->keyboardState.noteOff(1, note, 1.0);
            }
            break;
        }
        return;
    }

    switch (screen_state_) 
    {
    case ScreenState::Timeline:
        timeline_.HandleEvent(event);
        break;
    case ScreenState::Track:
        tracks_[current_track_]->HandleEvent(event);
        break;
    case ScreenState::PluginSelector:
        plugin_sel_.HandleEvent(event);
        break;
    }

}

} // namespace box
