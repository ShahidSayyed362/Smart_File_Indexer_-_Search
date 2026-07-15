#pragma once

#include <string>
#include <mutex>
#include <fstream>
#include <iostream>

namespace sfi::utils {

enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

class Logger {
public:
    static Logger& getInstance();

    // Disable copy/move
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void setLogFile(const std::string& filepath);
    void log(LogLevel level, const std::string& message);
    
    // Helper methods
    void info(const std::string& message);
    void warn(const std::string& message);
    void error(const std::string& message);
    void debug(const std::string& message);

private:
    Logger() = default;
    ~Logger() = default;

    std::string levelToString(LogLevel level) const;
    std::string currentDateTime() const;

    std::mutex m_mutex;
    std::ofstream m_fileStream;
};

} // namespace sfi::utils
