#include <tracktion_engine/tracktion_engine.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <ncurses.h>

namespace te = tracktion;
using namespace std::literals;
using namespace te::literals;

struct Note {
    int val_;
    bool on_;

    Note(int val, bool on = false): val_{val}, on_{on}{}
};

void playNote(te::AudioTrack *track, Note &note) {
    note.on_ = !note.on_;

    std::cout << "note.val_: " << note.val_ << std::endl;
    std::cout << "note.on_: " << note.on_ << std::endl;

    if (note.on_ == true) {
        auto message = juce::MidiMessage::noteOn (0, note.val_, (float) 100);
        track->injectLiveMidiMessage(message, 0);
    } else {
        auto message = juce::MidiMessage::noteOff(0, note.val_);
        track->injectLiveMidiMessage(message, 0);
    }
}

int main() {
    // Create the engine
    te::Engine engine{"Tracktion Hello World"};

    // Create an edit
    te::Edit edit{
        engine,
        te::createEmptyEdit(engine),
        te::Edit::forEditing,
        nullptr,
        0};

    // Create a track
    edit.ensureNumberOfAudioTracks(1);
    auto track = te::getAudioTracks(edit)[0];

    // Get length of 1 bar
    const tracktion::TimeRange oneBarTimeRange(
        0s,
        edit.tempoSequence.toTime({1, tracktion::BeatDuration()}));

    // Insert a 1 bar long Midi clip
    auto clip = track->insertNewClip(
        te::TrackItem::Type::midi,
        "Midi Clip",
        oneBarTimeRange,
        nullptr);
    auto midiClip = static_cast<te::MidiClip *>(clip);

    // Create a built-in synth plugin instance to play the sequence on
    auto plugin = edit.getPluginCache()
                      .createNewPlugin(te::FourOscPlugin::xmlTypeName, {})
                      .get();
    auto fourOscPlugin = static_cast<te::FourOscPlugin *>(plugin);

    // Insert the plugin to the track
    track->pluginList.insertPlugin(*fourOscPlugin, 0, nullptr);

    // Get the transport & set it to the start of the edit
    auto &transport = edit.getTransport();
    transport.setPosition(0s);

    // Begin playback
    transport.play(false);

    // Initialize ncurses
    initscr();              // Start ncurses mode
    cbreak();               // Disable line buffering
    noecho();               // Don't echo input characters
    keypad(stdscr, TRUE);   // Enable special keys (like arrow keys)
    nodelay(stdscr, TRUE);  // Make getch() non-blocking
   
    // Mapping keys to MIDI note numbers (assuming a starting note of C4, which is MIDI note 60)
    std::unordered_map<int, Note> keyToMIDINote = {
        {'a', 60},  // C4
        {'w', 61},  // C#4
        {'s', 62},  // D4
        {'e', 63},  // D#4
        {'d', 64},  // E4
        {'f', 65},  // F4
        {'t', 66},  // F#4
        {'g', 67},  // G4
        {'y', 68},  // G#4
        {'h', 69},  // A4
        {'u', 70},  // A#4
        {'j', 71},  // B4
        {'k', 72}   // C5
    };

    int ch;
    while ((ch = getch()) != 'q') {  // Exit on 'q' key press
        auto it = keyToMIDINote.find(ch);
        if (it != keyToMIDINote.end()) {
            playNote(track, it->second);
        }
        refresh();  // Update the screen
    }

    endwin();
    return 0;
}
