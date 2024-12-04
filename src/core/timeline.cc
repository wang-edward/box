#include "core/timeline.hh"
#include "core/app.hh"

namespace box {

Timeline:: Timeline()
{
    {
        Image tmp_l = LoadImage(METRONOME_PATH_L);
        metronome_l_ = LoadTextureFromImage(tmp_l);
        UnloadImage(tmp_l);
        Image tmp_r = LoadImage(METRONOME_PATH_R);
        metronome_r_ = LoadTextureFromImage(tmp_r);
        UnloadImage(tmp_r);
        Image tmp_off = LoadImage(METRONOME_PATH_OFF);
        metronome_off_ = LoadTextureFromImage(tmp_off);
        UnloadImage(tmp_off);
    }
}

Timeline:: ~Timeline()
{
    UnloadTexture(metronome_l_);
    UnloadTexture(metronome_r_);
    UnloadTexture(metronome_off_);
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
    assert(is_close(frame_.radius, radius_));

    const size_t num_rows = std::min(APP->tracks_.size() - scroll_offset_, MAX_TRACKS);
    const size_t curr_row = APP->GetCurrTrack() - scroll_offset_;

    const te::TransportControl &transport = APP->edit_.getTransport();
    const te::TempoSequence &tempo = APP->edit_.tempoSequence;

    const Position curr_pos = {
        static_cast<float>(APP->edit_.getTransport().getPosition().inSeconds()),
        static_cast<float>(tempo.toBeats(transport.getPosition()).inBeats())};

    BeatFrame screen{curr_pos.beats, radius_};

    if (playhead_mode_ == PlayheadMode::Detached)
    {
        screen = frame_;
    }

    assert(is_close(screen.radius, radius_));

    // draw track backgrounds
    for (size_t i = 0; i < num_rows; i++)
    {
        float x = 0;
        float y = (ROW_HEIGHT * i) + HEADER_HEIGHT;
        DrawRectangleRec(Rectangle{x, y, ROW_WIDTH, ROW_HEIGHT}, LIGHTGRAY);
    }

    // render active track
    assert(curr_row < num_rows);
    {
        float x = 0;
        float y = (ROW_HEIGHT * curr_row) + HEADER_HEIGHT;
        DrawRectangleLines(x, y, ROW_WIDTH, ROW_HEIGHT, RED);
    }

    // render pos
    {
        auto text = std::to_string(curr_pos.secs);

        Vector2 text_size = MeasureTextEx(GetFontDefault(), text.c_str(), HEADER_FONT_SIZE, HEADER_FONT_SIZE / DEFAULT_FONT_SIZE);
        DrawText(text.c_str(), (SCREEN_HALF - text_size.x/2), (HEADER_HEIGHT/2 - text_size.y/2), HEADER_FONT_SIZE, WHITE);
    }

    // render metronome
    {
        if (APP->edit_.clickTrackEnabled == true)
        {
            const int beat = static_cast<int>(curr_pos.beats);
            if (beat % 2 == 0)
            {
                DrawTexture(metronome_l_, SCREEN_EIGHTH/2 - ICON_RADIUS, HEADER_HEIGHT/2 - ICON_RADIUS, WHITE);
            }
            else
            {
                DrawTexture(metronome_r_, SCREEN_EIGHTH/2 - ICON_RADIUS, HEADER_HEIGHT/2 - ICON_RADIUS, WHITE);
            }
        }
        else
        {
            DrawTexture(metronome_off_, SCREEN_EIGHTH/2 - ICON_RADIUS, HEADER_HEIGHT/2 - ICON_RADIUS, WHITE);
        }
    }

    // render bar lines
    {
        float first_bar_start = std::floor(screen.LeftEdge() / bar_width_) * bar_width_;

        for (float bar_start = first_bar_start; 
            bar_start < screen.RightEdge();
            bar_start += bar_width_)
        {
            float bar_position_pct = (bar_start - screen.LeftEdge()) / screen.Width();
            int x = static_cast<int>(bar_position_pct * SCREEN_WIDTH);
            DrawLine(x, SCREEN_QUARTER, x, SCREEN_QUARTER + (ROW_HEIGHT * MAX_ROWS), DARKGRAY);
        }
    }

    // render current live clip (if recording)
    if (transport.isRecording())
    {
        const float start_time = tempo.toBeats(transport.getTimeWhenStarted()).inBeats();
        if (screen.LeftEdge() < start_time)
        {
            float left_pct = (start_time - screen.LeftEdge()) / screen.Width();
            float left_px = (left_pct * SCREEN_WIDTH);
            DrawRectangle(left_px, (curr_row * ROW_HEIGHT) + HEADER_HEIGHT, (SCREEN_HALF - left_px), ROW_HEIGHT, RED);
        }
        else {
            DrawRectangle(0, (curr_row * ROW_HEIGHT) + HEADER_HEIGHT, (SCREEN_HALF - 0), ROW_HEIGHT, RED); // 64 - 0 means full half bar
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
                BeatWindow clip = {
                    static_cast<float>(t_br.getStart().inBeats()), 
                    static_cast<float>(t_br.getLength().inBeats())};

                // Determine overlap with visible range
                const float visible_start = std::max(screen.LeftEdge(), clip.LeftEdge());
                const float visible_end = std::min(screen.RightEdge(), clip.RightEdge());

                if (visible_start < visible_end) // Clip is visible
                {
                    float start_pct = (visible_start - screen.LeftEdge()) / screen.Width();
                    float end_pct = (visible_end - screen.LeftEdge()) / screen.Width();

                    const int left_px = static_cast<int>(start_pct * SCREEN_WIDTH);
                    const int right_px = static_cast<int>(end_pct * SCREEN_WIDTH);
                    int clip_width = right_px - left_px;

                    DrawRectangle(left_px, (i * ROW_HEIGHT) + HEADER_HEIGHT, clip_width, ROW_HEIGHT, PINK);
                }
            }
        }
    }

    // render cursor
    if (!transport.isRecording() && !transport.isPlaying())
    {
        const float left_pct = (cursor_.LeftEdge() - screen.LeftEdge()) / screen.Width();
        const float right_pct = (cursor_.RightEdge() - screen.LeftEdge()) / screen.Width();

        const int left_px = static_cast<int>(left_pct * SCREEN_WIDTH);
        const int right_px = static_cast<int>(right_pct * SCREEN_WIDTH);
        const int width = right_px - left_px;

        DrawRectangleLines(left_px, (curr_row * ROW_HEIGHT) + HEADER_HEIGHT, width, ROW_HEIGHT, ORANGE);
    }

    // render playhead
    {
        if (screen.LeftEdge() < curr_pos.beats &&
            curr_pos.beats < screen.RightEdge())
        {
            const float left_pct = (curr_pos.beats - screen.LeftEdge()) / screen.Width();
            const int left_px = static_cast<int>(left_pct * SCREEN_WIDTH);
            DrawLine(left_px, HEADER_HEIGHT, left_px, SCREEN_HEIGHT, WHITE);
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
    const auto &track = APP->tracks_[APP->GetCurrTrack()]->base_;
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
                cursor_.start -= step_size_;
                LOG_VAR(cursor_.LeftEdge() < frame_.LeftEdge());
                if (cursor_.LeftEdge() < frame_.LeftEdge())
                {
                    const float diff = frame_.LeftEdge() - cursor_.LeftEdge();
                    frame_.center -= diff;
                }
                break;
            case KEY_L:
                cursor_.start += step_size_;
                LOG_VAR(cursor_.RightEdge() > frame_.RightEdge());
                if (cursor_.RightEdge() > frame_.RightEdge())
                {
                    const float diff = cursor_.RightEdge() - frame_.RightEdge();
                    frame_.center += diff;
                }
                break;
            case KEY_J:
                if (!APP->edit_.getTransport().isRecording())
                {
                    APP->SetCurrTrack(
                        clamp_increment(APP->GetCurrTrack(), APP->tracks_.size()));
                    if (APP->GetCurrTrack() - scroll_offset_ >= MAX_ROWS)
                    {
                        scroll_offset_ = clamp_increment(scroll_offset_, APP->tracks_.size());
                    }
                }
                break;
            case KEY_K:
                if (!APP->edit_.getTransport().isRecording())
                {
                    APP->SetCurrTrack(clamp_decrement(APP->GetCurrTrack()));
                    if (APP->GetCurrTrack() < scroll_offset_)
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
                print_timeline();
                LOG_VAR(frame_.center);
                break;
            case KEY_C:
                // toggle metronome
                APP->edit_.clickTrackEnabled = !APP->edit_.clickTrackEnabled;
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
                        playhead_mode_ = PlayheadMode::Detached;

                        {
                        const float curr_pos = APP->edit_.tempoSequence.toBeats(transport.getPosition()).inBeats();
                        cursor_.start = std::floor(curr_pos/bar_width_) * bar_width_;
                        frame_.center = curr_pos;
                        }
                        assert_multiple(cursor_.start, bar_width_);
                    }
                    else
                    {
                        transport.record(false);
                        playhead_mode_ = PlayheadMode::Locked;
                    }
                }
                break;
            case KEY_T:
                {
                    auto &transport = APP->edit_.getTransport();
                    if (transport.isPlaying())
                    {
                        transport.stop(false, false); // TODO should this discard?
                        playhead_mode_ = PlayheadMode::Detached;

                        {
                        const float curr_pos = APP->edit_.tempoSequence.toBeats(transport.getPosition()).inBeats();
                        cursor_.start = std::floor(curr_pos/bar_width_) * bar_width_;
                        frame_.center = curr_pos;
                        }
                        assert_multiple(cursor_.start, bar_width_);
                    }
                    else
                    {
                        transport.play(false);
                        playhead_mode_ = PlayheadMode::Locked;
                    }
                }
                break;
            case KEY_S:
                {
                    LOG_MSG("move to origin");
                    auto &transport = APP->edit_.getTransport();
                    transport.setPosition(te::TimePosition::fromSeconds(0.f));
                    cursor_.start = 0;
                    frame_.center = 0;
                }
                break;
            case KEY_Z:
                {
                    LOG_MSG("align cursor and playhead");
                    LOG_VAR(cursor_.start);
                    auto &transport = APP->edit_.getTransport();
                    const te::TempoSequence &tempo = APP->edit_.tempoSequence;
                    transport.setPosition(tempo.toTime(te::BeatPosition::fromBeats(cursor_.start)));
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
