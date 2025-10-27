// logger.h
#pragma once
#include <sstream>
#include <fstream>
#include <mutex>
#include <string>

enum LogLevel {
    INFO,
    WARN,
    ERR,
    DEBUG
};

class LoggerStream {
public:
    LoggerStream(LogLevel level,
        const char* file,
        const char* function,
        int line);
    ~LoggerStream();

    template<typename T>
    LoggerStream& operator<<(const T& value) {
        stream_ << value;
        return *this;
    }

    // Support std::endl and manipulators
    typedef std::ostream& (*StreamManipulator)(std::ostream&);
    LoggerStream& operator<<(StreamManipulator manip) {
        manip(stream_);
        return *this;
    }

private:
    LogLevel level_;
    std::ostringstream stream_;
    const char* file_;
    const char* function_;
    int line_;
};

// Main singleton Logger
class Logger {
public:
    static Logger& getInstance();
    void write(LogLevel level, const std::string& message,
        const char* file, const char* function, int line);

private:
    Logger();
    ~Logger();

    std::ofstream logFile_;
    std::mutex mutex_;
};

// Entry macro
#define log(level) LoggerStream(level, __FILE__, __FUNCTION__, __LINE__)
