#include "core/util.hh"

namespace box
{

App *APP = nullptr; // Define the global APP variable

void assert_multiple(float a, float b)
{
    const float epsilon = 1e-6; // Small tolerance for floating-point precision
    assert(std::fabs(std::fmod(a, b)) < epsilon && "a is not a multiple of b");
}

bool is_close(float a, float b)
{
    return std::abs(a - b) < 1e-6;
}

std::string format_time(float time)
{
    int minutes = static_cast<int>(time) / 60;
    int seconds = static_cast<int>(time) % 60;
    int decimal = static_cast<int>(std::fmod(time, 1.0f) * 100); // Two decimal places

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << minutes << "." << std::setw(2) << std::setfill('0') << seconds << "."
        << std::setw(2) << std::setfill('0') << decimal;

    return oss.str();
}

std::string format_key_offset(int key_offset)
{
    if (key_offset > 0)
        return "+" + std::to_string(key_offset);
    else
        return std::to_string(key_offset);
}

std::string format_cursor_pos(size_t track_pos, float cursor_pos)
{
    std::ostringstream oss;
    oss << std::setw(1) << track_pos << ":" << std::setw(3) << std::setfill('0') << cursor_pos;
    return oss.str();
}

size_t clamp_decrement(size_t x)
{
    if (x == 0)
        return 0;
    else
        return x - 1;
}

size_t clamp_increment(size_t x, size_t max)
{
    return std::min(x + 1, clamp_decrement(max));
}

te::Clip *findCurrClip(te::AudioTrack &track, te::TimePosition time)
{
    for (int i = 0; i < track.getNumTrackItems(); i++)
    {
        te::TrackItem *item = track.getTrackItem(i);
        auto pos = item->getPosition().time;
        if (pos.getStart() < time && pos.getEnd() > time)
        {
            return dynamic_cast<te::Clip *>(item);
        }
    }
    return nullptr;
}

te::Clip *findPrevClip(te::AudioTrack &track, te::TimePosition time)
{
    // iterate in reverse and find the first track before
    int n = track.getNumTrackItems();
    for (int i = n - 1; i >= 0; i--)
    {
        te::TrackItem *item = track.getTrackItem(i);
        auto pos = item->getPosition().time;
        if (pos.getEnd() < time)
        {
            return dynamic_cast<te::Clip *>(item);
        }
    }
    return nullptr;
}

LogLevel string_to_loglevel(const std::string &s)
{
    if (s == "OFF")
        return LogLevel::Off;
    if (s == "DEBUG")
        return LogLevel::Debug;
    if (s == "WARN")
        return LogLevel::Warn;
    if (s == "ERR")
        return LogLevel::Err;
    throw std::runtime_error{"string_to_loglevel(" + s + ") not found"};
}

std::string loglevel_to_string(LogLevel l)
{
    if (l == LogLevel::Off)
        return "OFF";
    if (l == LogLevel::Debug)
        return "DEBUG";
    if (l == LogLevel::Warn)
        return "WARN";
    if (l == LogLevel::Err)
        return "ERR";
    throw std::runtime_error{"loglevel_to_string(" + loglevel_to_string(l) + ") not found"};
}

// get loglevel from env
LogLevel get_loglevel()
{
    const char *env = std::getenv("BOX_LOG");
    if (env)
    {
        return string_to_loglevel(env);
    }
    return LogLevel::Off; // default
}

void LOG_MSG(const std::string &msg, LogLevel l)
{
    static LogLevel curr = get_loglevel();

    if (l < curr)
    {
        return;
    }
    // get time
    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    // print
    std::cout << "[" << buf << "] " << loglevel_to_string(l) << ": " << msg << std::endl;
}

} // namespace box
