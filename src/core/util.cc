#include "core/util.hh"

namespace box {

LogLevel string_to_loglevel(const std::string& s) {
    if (s == "OFF") return LogLevel::Off;
    if (s == "DEBUG") return LogLevel::Debug;
    if (s == "WARN") return LogLevel::Warn;
    if (s == "ERR") return LogLevel::Err;
    throw std::runtime_error{"string_to_loglevel(" + s + ") not found"};
}

std::string loglevel_to_string(LogLevel l) {
    if (l == LogLevel::Off) return "OFF";
    if (l == LogLevel::Debug) return "DEBUG";
    if (l == LogLevel::Warn) return "WARN";
    if (l == LogLevel::Err) return "ERR";
    throw std::runtime_error{"loglevel_to_string(" + loglevel_to_string(l) + ") not found"};
}

// get loglevel from env
LogLevel get_loglevel() {
    const char* env = std::getenv("BOX_LOG");
    if (env) {
        return string_to_loglevel(env);
    }
    return LogLevel::Off; // default
}

void LOG_MSG(const std::string& msg, LogLevel l) {
    static LogLevel curr = get_loglevel();

    if (l < curr) {
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
