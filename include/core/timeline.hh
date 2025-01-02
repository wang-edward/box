#pragma once

#include "interface.hh"
#include "util.hh"

namespace box
{

struct Timeline
{
    enum class ScreenState
    {
        Overview,
    };

    enum class PlayheadMode
    {
        Detached,
        Locked,
    };

    struct BeatWindow
    {
        float start;
        float len;
        float LeftEdge() const
        {
            return start;
        }
        float RightEdge() const
        {
            return start + len;
        }
    };

    struct BeatFrame
    {
        float center;
        float radius;
        float LeftEdge() const
        {
            return center - radius;
        }
        float RightEdge() const
        {
            return center + radius;
        }
        float Width() const
        {
            return radius * 2;
        }
        BeatFrame(float c, float &r) : center{c}, radius{r}
        {
        }
    };

    struct Position
    {
        float secs;
        float beats;
    };

    float bar_width_ = 4.0; // Assuming a 4/4 time signature by default TODO
                            // update with time signature changes
    float step_size_ = 4.0; // Move cursor by 4 beat (1 bar)
    float radius_ = 8.0;
    BeatWindow cursor_ = {0.0, bar_width_};
    BeatFrame frame_ = {0.0, radius_};

    ScreenState screen_state_ = ScreenState::Overview;
    PlayheadMode playhead_mode_ = PlayheadMode::Detached;
    size_t scroll_offset_ = 0;

    static constexpr const char *METRONOME_PATH_L = "assets/metronome_l.png";
    static constexpr const char *METRONOME_PATH_R = "assets/metronome_r.png";
    static constexpr const char *METRONOME_PATH_OFF =
        "assets/metronome_off.png";
    Texture2D metronome_l_;
    Texture2D metronome_r_;
    Texture2D metronome_off_;

    Timeline();
    ~Timeline();

    void FocusCursor();

    void Render(Interface &interface);
    void HandleEvent(const Event &event);

  private:
    void print_timeline();
};

} // namespace box
