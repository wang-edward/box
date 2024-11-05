#include "core/timeline.hh"
#include "core/app.hh"

namespace box {

void Timeline:: Render(Interface &interface) 
{
    for (size_t i = 0; i < APP->tracks_.size(); i++) 
    {
        float x = 0;
        float y = (24 * i) + 32;
        float width = 128;
        float height = 24;
        DrawRectangleRec(Rectangle{x, y, width, height}, APP->colors_[i]);
    }

    // render active track
    {
        float x = 0;
        float y = (24 * APP->current_track_) + 32;
        float width = 128;
        float height = 24;
        DrawRectangleRec(Rectangle{x, y, width, height}, {0xff, 0xff, 0xff, 0x80});
    }

    // render bar
    {
        double pos = APP->edit_.getTransport().getPosition().inSeconds();
        auto text = std::to_string(pos);
        const int font_size = 10;
        int width = MeasureText(text.c_str(), font_size);
        DrawText(text.c_str(), (64 - width/2), 16, 10, WHITE);
    }
}

inline bool isTrackArmed (te::AudioTrack& t, int position = 0)
{
    auto& edit = t.edit;
    for (auto instance : edit.getAllInputDevices())
        if (instance->isOnTargetTrack (t, position))
            return instance->isRecordingEnabled (t);

    return false;
}

inline bool trackHasInput (te::AudioTrack& t, int position = 0)
{
    auto& edit = t.edit;
    for (auto instance : edit.getAllInputDevices())
        if (instance->isOnTargetTrack (t, position))
            return true;

    return false;
}

void Timeline:: HandleEvent(const Event &event) 
{
    switch (screen_state_) 
    {
    case ScreenState::Overview:
        switch (event.type) 
        {
        case EventType::KeyPress:
            switch (event.value) 
            {
            case KEY_ENTER:
                APP->screen_state_ = App::ScreenState::Track;
                break;
            case KEY_J:
                APP->current_track_ = std::min(APP->current_track_ + 1, clamp_decrement(APP->tracks_.size()));
                break;
            case KEY_K:
                APP->current_track_ = clamp_decrement(APP->current_track_);
                break;
            case KEY_O: // add track
                if (APP->tracks_.size() < MAX_TRACKS) 
                {
                    APP->AddTrack();
                }
                break;
            case KEY_P:
                LOG_VAR(APP->edit_.getTransport().getCurrentPosition());
                LOG_VAR(APP->tracks_[APP->current_track_]->track_.getClips().size());
                LOG_VAR(isTrackArmed(APP->tracks_[APP->current_track_]->track_));
                LOG_VAR(trackHasInput(APP->tracks_[APP->current_track_]->track_));
                break;
            case KEY_R:
                {
                    auto &transport = APP->edit_.getTransport();
                    LOG_VAR(transport.getCurrentPosition());
                    LOG_VAR(transport.isRecording());
                    if (transport.isRecording())
                    {
                        transport.stop(false, false); // TODO should this discard?
                        te::EditFileOperations (APP->edit_).save(true, true, false);
                    }
                    else
                    {
                        transport.record(false);
                    }
                }
                break;
            case KEY_T:
                auto &transport = APP->edit_.getTransport();
                if (transport.isPlaying())
                {
                    transport.stop(false, false); // TODO should this discard?
                }
                else
                {
                    transport.play(false);
                }
            }
            break;
        }
        break;
    }
}

} // namespace box
