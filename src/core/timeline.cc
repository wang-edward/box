#include "core/timeline.hh"
#include "core/app.hh"

namespace box {

Timeline:: Timeline()
{
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
    const float RADIUS = radius_;
    const float WIDTH = radius_ * 2;

    const te::TransportControl &transport = APP->edit_.getTransport();
    const te::TempoSequence &tempo = APP->edit_.tempoSequence;

    const Position curr_pos = {
        static_cast<float>(APP->edit_.getTransport().getPosition().inSeconds()),
        static_cast<float>(te::toBeats(te::EditTime{transport.getPosition()}, tempo).inBeats())};

    BeatRange screen = {
        curr_pos.beats - RADIUS,
        curr_pos.beats + RADIUS};

    if (playhead_mode_ == PlayheadMode::Detached)
    {
        screen = frame_;
    } else if (playhead_mode_ == PlayheadMode::Locked)
    {
        screen.left_edge = curr_pos.beats - RADIUS;
        screen.right_edge = curr_pos.beats + RADIUS;
    }

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
    assert(curr_row < num_rows);
    {
        float x = 0;
        float y = (24 * curr_row) + 32;
        float width = 128;
        float height = 24;
        DrawRectangleLines(x, y, width, height, RED);
    }

    // render pos
    {
        auto text = std::to_string(curr_pos.secs);
        const int font_size = 10;
        int width = MeasureText(text.c_str(), font_size);
        DrawText(text.c_str(), (64 - width/2), 16, 10, WHITE);
    }

    // render bar lines
    {
        float first_bar_start = std::floor(screen.left_edge / bar_width_) * bar_width_;

        for (float bar_start = first_bar_start; 
            bar_start < screen.right_edge; 
            bar_start += bar_width_)
        {
            float bar_position_pct = (bar_start - screen.left_edge) / WIDTH;
            int x = static_cast<int>(bar_position_pct * 128);
            DrawLine(x, 32, x, 32 + (24 * 4), DARKGRAY);
        }
    }

    // render current live clip (if recording)
    if (transport.isRecording())
    {
        const float start_time = tempo.toBeats(transport.getTimeWhenStarted()).inBeats();
        if (screen.left_edge < start_time)
        {
            float left_pct = (start_time - screen.left_edge) / WIDTH;
            float left_px = (left_pct * 128);
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
                te::BeatRange t_br = te::toBeats(c_pos.time, tempo);
                BeatRange clip = {
                    static_cast<float>(t_br.getStart().inBeats()), 
                    static_cast<float>(t_br.getEnd().inBeats())};

                // Determine overlap with visible range
                const float visible_start = std::max(screen.left_edge, clip.left_edge);
                const float visible_end = std::min(screen.right_edge, clip.right_edge);

                if (visible_start < visible_end) // Clip is visible
                {
                    float start_pct = (visible_start - screen.left_edge) / WIDTH;
                    float end_pct = (visible_end - screen.left_edge) / WIDTH;

                    const int left_px = static_cast<int>(start_pct * 128);
                    const int right_px = static_cast<int>(end_pct * 128);
                    int clip_width = right_px - left_px;

                    DrawRectangle(left_px, (i * 24) + 32, clip_width, 24, PINK);
                }
            }
        }
    }

    // render cursor
    {
        const float left_pct = (cursor_.left_edge - screen.left_edge) / WIDTH;
        const float right_pct = (cursor_.right_edge - screen.left_edge) / WIDTH;

        const int left_px = static_cast<int>(left_pct * 128);
        const int right_px = static_cast<int>(right_pct * 128);
        const int width = right_px - left_px;

        DrawRectangleLines(left_px, (curr_row * 24) + 32, width, 24, ORANGE);
    }

    // render playhead
    {
        if (screen.left_edge < curr_pos.beats &&
            curr_pos.beats < screen.right_edge)
        {
            const float left_pct = (curr_pos.beats - screen.left_edge) / WIDTH;
            const int left_px = static_cast<int>(left_pct * 128);
            DrawLine(left_px, 32, left_px, 128, WHITE);
        }
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
            case KEY_H:
                cursor_.left_edge -= step_size_;
                cursor_.right_edge -= step_size_;
                if (cursor_.left_edge < frame_.left_edge)
                {
                    frame_.left_edge = cursor_.left_edge;
                    frame_.right_edge = cursor_.left_edge + (radius_ * 2);
                }
                break;
            case KEY_L:
                cursor_.left_edge += step_size_;
                cursor_.right_edge += step_size_;
                if (cursor_.right_edge > frame_.right_edge)
                {
                    frame_.left_edge = cursor_.right_edge - (radius_ * 2);
                    frame_.right_edge = cursor_.right_edge;
                }
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
                frame_.left_edge *= 2;
                frame_.right_edge *= 2;
                break;
            case KEY_EQUAL:
                radius_ /= 2;
                frame_.left_edge /= 2;
                frame_.right_edge /= 2;
                break;
            }
            break;
        }
        break;
    }
}

} // namespace box
