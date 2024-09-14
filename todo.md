# doing?
- UI
- use te::Track inheritance instead of te::AudioTrack

# SHA
- Rewrite Parameter to be better
- has getNorm for CV

# todo
## easy

## long term
- patchbay?

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
