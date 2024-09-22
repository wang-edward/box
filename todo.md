# doing?
- trying to find a good way to manage state
    - i think callbacks can be good?
    - my problem is that Manager holds multiple screens, like the Tracks, Timeline, PluginSelector
    - I can dispatch events and render easily downwards, but I don't know how to go the other way
        - like modifying the state of Manager from a subcomponent
    - The easiest way is having a reference back to Manager, but i think that's really really bad from a design standpoint
- UI

# SHA
- Rewrite Parameter to be better
- has getNorm for CV

# todo
## easy
- make const vectors arrays

## long term
- patchbay?
- distinction between instrument and effect? like having seperate slots for types of plugins
- better way to manage colorschemes - MAINCOLOR, ALT1, ALT2, etc.
- better way to manage plugins - define stuff all in one place

# timeline
- timer turns red when recording
- timer turns grey when all tracks are muted
- timer turns yellow when a track is solo'ed

## how do tracks work?
inheritance
- Track
    - AutomationTrack
    - ChordTrack
    - ClipTrack
        - ArrangerTrack
        - AudioTrack
        - MarkerTrack
    - FolderTrack
    - MasterTrack
    - TempoTrack
- TrackCompManager
- EditItem

How does TrackOutputWork?
Example
```cpp
void AudioTrack::injectLiveMidiMessage (const MidiMessageArray::MidiMessageWithSource& message)
{
    TRACKTION_ASSERT_MESSAGE_THREAD
    bool wasUsed = false;
    listeners.call (&Listener::injectLiveMidiMessage, *this, message, wasUsed);

    if (! wasUsed)
        edit.warnOfWastedMidiMessages (nullptr, this);
}
```
- TrackOutput is set up as a Listener to AudioTrack???
- and then when this is called TrackOutput gets it and thats what makes the sound?
- i'm just tokenizing words bro
- dis jawn a bit complicated
