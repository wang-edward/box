#include "core/timeline.hh"
#include "core/app.hh"

namespace box {

Timeline:: Timeline()
{
    cursor_ = {
        te::BeatPosition::fromBeats(0.0),
        te::BeatPosition::fromBeats(bar_width_)};
}

void Timeline:: Render(Interface &interface) 
{
    /*
    terminology:
    - row = the relative index of a track.
      - so the top rectangle is always row 0, no matter what track it is
    - track
      - the actual "track"
    */

    const size_t num_rows = std::min(APP->tracks_.size() - scroll_offset_, MAX_TRACKS);
    const size_t curr_row = APP->current_track_ - scroll_offset_;
    const double RADIUS = radius_;
    const double WIDTH = radius_ * 2;
    const double pos = APP->edit_.getTransport().getPosition().inSeconds();

    const te::TransportControl &transport = APP->edit_.getTransport();
    const te::TempoSequence &tempo = APP->edit_.tempoSequence;
    const te::BeatPosition &curr_beat_pos = te::toBeats(te::EditTime{transport.getPosition()}, tempo);

    const auto screen_left_edge = te::BeatPosition::fromBeats(curr_beat_pos.inBeats() - RADIUS);
    const auto screen_right_edge = te::BeatPosition::fromBeats(curr_beat_pos.inBeats() + RADIUS);

    // draw track backgrounds
    for (size_t i = 0; i < num_rows; i++)
    {
        float x = 0;
        float y = (24 * i) + 32;
        float width = 128;
        float height = 24;
        DrawRectangleRec(Rectangle{x, y, width, height}, LIGHTGRAY);
    }

    // render active track
    if (curr_row < num_rows)
    {
        float x = 0;
        float y = (24 * curr_row) + 32;
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
        double first_bar_start = std::floor(screen_left_edge.inBeats() / beats_per_bar) * beats_per_bar;

        for (double bar_start = first_bar_start; bar_start < screen_right_edge.inBeats(); bar_start += beats_per_bar)
        {
            double bar_position_pct = (bar_start - screen_left_edge.inBeats()) / WIDTH;
            float x = static_cast<float>(bar_position_pct * 128);
            DrawLine(x, 32, x, 32 + (24 * 4), DARKGRAY); // Full-height bar line
        }
    }

    // render current live clip (if recording)
    if (transport.isRecording())
    {
        const te::BeatPosition start_time = tempo.toBeats(transport.getTimeWhenStarted());
        if (screen_left_edge < start_time)
        {
            double left_pct = (start_time.inBeats() - screen_left_edge.inBeats()) / WIDTH;
            double left_px = (left_pct * 128);
            DrawRectangle(left_px, (curr_row * 24) + 32, (64 - left_px), 24, RED);
        }
        else {
            DrawRectangle(0, (curr_row * 24) + 32, (64 - 0), 24, RED); // 64 - 0 means full half bar
        }
    }

    // render clips
    {
        for (size_t i = 0; i < num_rows; i++)
        {
            const size_t track_idx = i + scroll_offset_;
            const auto &t = APP->tracks_[track_idx];
            for (const auto &c : t->base_.getClips())
            {
                const te::ClipPosition c_pos = c->getPosition();
                auto c_er = te::EditTimeRange{c_pos.time};
                te::BeatRange t_br = te::toBeats(c_er, tempo);

                // Determine overlap with visible range
                auto visible_start = std::max(screen_left_edge.inBeats(), t_br.getStart().inBeats());
                auto visible_end = std::min(screen_right_edge.inBeats(), t_br.getEnd().inBeats());

                if (visible_start < visible_end) // Clip is visible
                {
                    double start_pct = (visible_start - screen_left_edge.inBeats()) / WIDTH;
                    double end_pct = (visible_end - screen_left_edge.inBeats()) / WIDTH;

                    float left_px = static_cast<float>(start_pct * 128);
                    float right_px = static_cast<float>(end_pct * 128);
                    float width = right_px - left_px;

                    DrawRectangle(left_px, (i * 24) + 32, width, 24, PINK);
                }
            }
        }
    }

    // render cursor
    {
        const te::BeatPosition cursor_left_edge = cursor_.getStart();
        const te::BeatPosition cursor_right_edge = cursor_.getEnd();

        double left_pct = (cursor_left_edge.inBeats() - screen_left_edge.inBeats()) / WIDTH;
        double right_pct = (cursor_right_edge.inBeats() - screen_left_edge.inBeats()) / WIDTH;

        float left_px = static_cast<float>(left_pct * 128);
        float right_px = static_cast<float>(right_pct * 128);
        float width = right_px - left_px;

        DrawRectangle(left_px, (curr_row * 24) + 32, width, 24, ORANGE);
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
            // TODO this code is stupid fix it
            case KEY_H:
                cursor_ = te::BeatRange{
                    te::BeatPosition::fromBeats(
                        cursor_.getStart().inBeats() - step_size_),
                    te::BeatPosition::fromBeats(
                        cursor_.getEnd().inBeats() - step_size_)};
                LOG_VAR(cursor_.getStart().inBeats());
                break;
            case KEY_L:
                cursor_ = te::BeatRange{
                    te::BeatPosition::fromBeats(
                        cursor_.getStart().inBeats() + step_size_),
                    te::BeatPosition::fromBeats(
                        cursor_.getEnd().inBeats() + step_size_)};
                LOG_VAR(cursor_.getStart().inBeats());
                break;
            case KEY_J:
                {
                    const size_t num_rows = std::min(APP->tracks_.size() - scroll_offset_, MAX_TRACKS);
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
