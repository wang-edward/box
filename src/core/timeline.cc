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

    // render pos
    {
        double pos = APP->edit_.getTransport().getPosition().inSeconds();
        auto text = std::to_string(pos);
        const int font_size = 10;
        int width = MeasureText(text.c_str(), font_size);
        DrawText(text.c_str(), (64 - width/2), 16, 10, WHITE);
    }

    // render cursor
    {
        DrawLine(64, 0, 64, 128, WHITE);
    }

    // render clips
    {
        // render everything in -4, +4 beats

        constexpr double RADIUS = 4.0;
        constexpr double WIDTH = RADIUS * 2;

        const te::TransportControl &transport = APP->edit_.getTransport();
        const te::TempoSequence &tempo = APP->edit_.tempoSequence;
        const te::BeatPosition &curr_beat_pos = te::toBeats(te::EditTime{transport.getPosition()}, tempo);
        auto left = te::BeatPosition::fromBeats(curr_beat_pos.inBeats() - RADIUS);
        auto right = te::BeatPosition::fromBeats(curr_beat_pos.inBeats() + RADIUS);
        int cnt = 0;
        for (const auto &t : APP->tracks_)
        {
            // TODO make this work with scroll
            int y = (24 * cnt) + 32;
            for (const auto &c : t->base_.getClips())
            {
                const te::ClipPosition c_pos = c->getPosition();
                auto c_er = te::EditTimeRange{c_pos.time};
                te::BeatRange t_br = te::toBeats(c_er, tempo);

                auto left_dist = t_br.getStart().inBeats() - left.inBeats();
                auto right_dist = right.inBeats() - t_br.getEnd().inBeats();

                auto left_pct = left_dist / WIDTH;
                auto right_pct = right_dist / WIDTH;

                auto left_px = left_pct * 128;
                auto right_px = 128 - right_pct * 128;

                DrawRectangle(left_px, y, right_px, 24, RED);

                // print_timeline();
                // LOG_VAR(left_dist);
                // LOG_VAR(right_dist);

                // TODO if (t_br.getStart() < right || t_br.getEnd() > left)
            }
            cnt++;
        }
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

void Timeline:: print_timeline()
{
    std::cout << std::endl;
    const auto &track = APP->tracks_[APP->current_track_]->base_;
    std::cout << "name: " << track.getName() << std::endl;
    std::cout << "isMuted: " << track.isMuted(false) << std::endl;
    std::cout << "isSolo: " << track.isSolo(false) << std::endl;

    const te::TempoSequence &tempo = APP->edit_.tempoSequence;
    const te::TransportControl &transport = APP->edit_.getTransport();
    const te::TimePosition &curr_time_pos = transport.getPosition();
    const te::BeatPosition &curr_beat_pos = te::toBeats(te::EditTime{transport.getPosition()}, tempo);
    const te::TimeSigSetting &time_sig = tempo.getTimeSigAt(curr_time_pos);

    std::cout << "TIME curr position: " << curr_time_pos.inSeconds() << std::endl;
    std::cout << "BEATS curr position: " << curr_beat_pos.inBeats() << std::endl;
    std::cout << "bpm: " << tempo.getBpmAt(curr_time_pos) << std::endl;
    std::cout << "time signature: " << time_sig.getStringTimeSig() << std::endl;

    const auto clips = track.getClips();
    std::cout << "num clips: " << clips.size() << std::endl;
    for (auto c : clips)
    {
        std::cout << "\t"<< "name: " << c->getName() << std::endl;
        std::cout << "\t\t"<< "isMidi: " << c->isMidi() << std::endl;
        const te::ClipPosition pos = c->getPosition();
        auto edit_range = te::EditTimeRange{pos.time};
        te::TimeRange time_range = te::toTime(edit_range, APP->edit_.tempoSequence);
        te::BeatRange beat_range = te::toBeats(edit_range, APP->edit_.tempoSequence);
        std::cout << "\t\t" << "TIME start: " << time_range.getStart().inSeconds() << std::endl;
        std::cout << "\t\t" << "TIME end: " << time_range.getEnd().inSeconds() << std::endl;
        std::cout << "\t\t" << "TIME length: " << pos.getLength().inSeconds() << std::endl;
        std::cout << "\t\t" << "BEATS start: " << beat_range.getStart().inBeats() << std::endl;
        std::cout << "\t\t" << "BEATS end: " << beat_range.getEnd().inBeats() << std::endl;
        std::cout << "\t\t" << "BEATS length: " << beat_range.getLength().inBeats() << std::endl;
    }
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
                LOG_VAR(APP->tracks_[APP->current_track_]->base_.getClips().size());
                LOG_VAR(isTrackArmed(APP->tracks_[APP->current_track_]->base_));
                LOG_VAR(trackHasInput(APP->tracks_[APP->current_track_]->base_));
                print_timeline();
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
                {
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
            case KEY_S:
                {
                    LOG_MSG("move to origin");
                    auto &transport = APP->edit_.getTransport();
                    transport.setPosition(te::TimePosition::fromSeconds(0.f));
                }
                break;
            case KEY_COMMA:
                {
                    LOG_MSG("move left");
                    auto &transport = APP->edit_.getTransport();
                    transport.setPosition(te::TimePosition::fromSeconds(transport.getCurrentPosition() - 2.f));
                }
                break;
            case KEY_PERIOD:
                {
                    LOG_MSG("move right");
                    auto &transport = APP->edit_.getTransport();
                    transport.setPosition(te::TimePosition::fromSeconds(transport.getCurrentPosition() + 2.f));
                }
                break;
            }
            break;
        }
        break;
    }
}

} // namespace box
