#include "core/timeline.hh"
#include "core/app.hh"

namespace box {

void Timeline:: Render(Interface &interface) 
{
    const size_t num_visible_tracks_ = std::min(APP->tracks_.size() - scroll_offset_, MAX_TRACKS);
    const size_t visible_active_index = APP->current_track_ - scroll_offset_;
    const double RADIUS = radius_;
    const double WIDTH = radius_ * 2;
    const double pos = APP->edit_.getTransport().getPosition().inSeconds();

    const te::TransportControl &transport = APP->edit_.getTransport();
    const te::TempoSequence &tempo = APP->edit_.tempoSequence;
    const te::BeatPosition &curr_beat_pos = te::toBeats(te::EditTime{transport.getPosition()}, tempo);

    const auto left_edge = te::BeatPosition::fromBeats(curr_beat_pos.inBeats() - RADIUS);
    const auto right_edge = te::BeatPosition::fromBeats(curr_beat_pos.inBeats() + RADIUS);

    // draw track backgrounds
    for (size_t i = 0; i < num_visible_tracks_; i++)
    {
        float x = 0;
        float y = (24 * i) + 32;
        float width = 128;
        float height = 24;
        DrawRectangleRec(Rectangle{x, y, width, height}, LIGHTGRAY);
    }

    // render active track
    if (visible_active_index < num_visible_tracks_)
    {
        float x = 0;
        float y = (24 * visible_active_index) + 32;
        float width = 128;
        float height = 24;
        DrawRectangleLines(x, y, width, height, RED);
    }

    // render pos
    {
        auto text = std::to_string(pos);
        const int font_size = 10;
        int width = MeasureText(text.c_str(), font_size);
        DrawText(text.c_str(), (64 - width/2), 16, 10, WHITE);
    }

    // render bar lines
    {

        // Time signature at current position

        const te::TimeSigSetting &time_sig = tempo.getTimeSigAt(transport.getPosition());
        double beats_per_bar = time_sig.numerator; // Number of beats in a bar
        double beat_length = 1.0 / time_sig.denominator; // Length of a beat in terms of whole notes

        // Start rendering bar lines from the leftmost visible beat
        double first_bar_start = std::floor(left_edge.inBeats() / beats_per_bar) * beats_per_bar;

        for (double bar_start = first_bar_start; bar_start < right_edge.inBeats(); bar_start += beats_per_bar)
        {
            double bar_position_pct = (bar_start - left_edge.inBeats()) / WIDTH;
            float x = static_cast<float>(bar_position_pct * 128);
            DrawLine(x, 32, x, 32 + (24 * 4), DARKGRAY); // Full-height bar line
        }
    }

    // render clips
    {
        for (size_t i = 0; i < num_visible_tracks_; i++)
        {
            const size_t track_idx = i + scroll_offset_;
            const auto &t = APP->tracks_[track_idx];
            for (const auto &c : t->base_.getClips())
            {
                const te::ClipPosition c_pos = c->getPosition();
                auto c_er = te::EditTimeRange{c_pos.time};
                te::BeatRange t_br = te::toBeats(c_er, tempo);

                // Determine overlap with visible range
                auto visible_start = std::max(left_edge.inBeats(), t_br.getStart().inBeats());
                auto visible_end = std::min(right_edge.inBeats(), t_br.getEnd().inBeats());

                if (visible_start < visible_end) // Clip is visible
                {
                    double start_pct = (visible_start - left_edge.inBeats()) / WIDTH;
                    double end_pct = (visible_end - left_edge.inBeats()) / WIDTH;

                    float left_px = static_cast<float>(start_pct * 128);
                    float right_px = static_cast<float>(end_pct * 128);
                    float width = right_px - left_px;

                    DrawRectangle(left_px, (i * 24) + 32, width, 24, RED);
                }
            }
        }
    }

    // render cursor
    {
        DrawLine(64, 0, 64, 128, WHITE);
    }

}

// TODO update?
// inline bool isTrackArmed (te::AudioTrack& t, int position = 0)
// {
//     auto& edit = t.edit;
//     for (auto instance : edit.getAllInputDevices())
//         if (instance->isOnTargetTrack (t, position))
//             return instance->isRecordingEnabled (t);

//     return false;
// }

// inline bool trackHasInput (te::AudioTrack& t, int position = 0)
// {
//     auto& edit = t.edit;
//     for (auto instance : edit.getAllInputDevices())
//         if (instance->isOnTargetTrack (t, position))
//             return true;

//     return false;
// }

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
    std::cout << "time signature numerator: " << time_sig.numerator << std::endl;
    std::cout << "time signature denominator: " << time_sig.denominator << std::endl;

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
                {
                    const size_t num_visible_tracks_ = std::min(APP->tracks_.size() - scroll_offset_, MAX_TRACKS);
                    size_t old_track = APP->current_track_;
                    APP->current_track_ = clamp_increment(APP->current_track_, APP->tracks_.size());
                    APP->UnarmMidi(old_track);
                    APP->ArmMidi(APP->current_track_);
                    // update scroll
                    LOG_VAR(APP->current_track_);
                    LOG_VAR(scroll_offset_);
                    LOG_VAR(APP->current_track_  - scroll_offset_);
                    if (APP->current_track_ - scroll_offset_ >= 4)
                    {
                        scroll_offset_ = clamp_increment(scroll_offset_, APP->tracks_.size());
                    }
                }
                break;
            case KEY_K:
                {
                    size_t old_track = APP->current_track_;
                    APP->current_track_ = clamp_decrement(APP->current_track_);
                    APP->UnarmMidi(old_track);
                    APP->ArmMidi(APP->current_track_);
                    // update scroll
                    LOG_VAR(APP->current_track_);
                    LOG_VAR(scroll_offset_);
                    if (APP->current_track_ < scroll_offset_)
                    {
                        scroll_offset_ = clamp_decrement(scroll_offset_);
                    }
                }
                break;
            case KEY_UP:
                scroll_offset_ = clamp_decrement(scroll_offset_);
                break;
            case KEY_DOWN:
                scroll_offset_ = clamp_increment(scroll_offset_, APP->tracks_.size());
                break;
            case KEY_O: // add track
                if (APP->tracks_.size() < MAX_TRACKS) 
                {
                    APP->AddTrack();
                }
                break;
            case KEY_P:
                // TODO update?
                print_timeline();
                LOG_VAR(APP->tracks_.size() - scroll_offset_);
                LOG_VAR(std::min(APP->tracks_.size() - scroll_offset_, MAX_TRACKS));
                break;
            case KEY_R:
                {
                    auto &transport = APP->edit_.getTransport();
                    LOG_VAR(transport.getPosition().inSeconds());
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
                    transport.setPosition(te::TimePosition::fromSeconds(transport.getPosition().inSeconds() - 2.f));
                }
                break;
            case KEY_PERIOD:
                {
                    LOG_MSG("move right");
                    auto &transport = APP->edit_.getTransport();
                    transport.setPosition(te::TimePosition::fromSeconds(transport.getPosition().inSeconds() + 2.f));
                }
                break;
            case KEY_MINUS:
                radius_ *= 2;
                break;
            case KEY_EQUAL:
                radius_ /= 2;
                break;
            }
            break;
        }
        break;
    }
}

} // namespace box
