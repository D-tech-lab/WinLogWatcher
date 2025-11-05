#include "logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <thread>
#include <chrono>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <direct.h>     // For _mkdir
#include <Windows.h>
using namespace std;
//namespace fs = std::filesystem;

static std::string getLevelString(LogLevel level) {
    switch (level) {
    case INFO: return "INFO";
    case WARN: return "WARN";
    case ERR: return "ERROR";
    case DEBUG: return "DEBUG";
    default: return "UNKNOWN";
    }
}
LoggerStream::LoggerStream(LogLevel level,
    const char* file,
    const char* function,
    int line)
    : level_(level), file_(file), function_(function), line_(line) {
}

LoggerStream::~LoggerStream() {
    Logger::getInstance().write(level_, stream_.str(), file_, function_, line_);
    stream_.str("");
    stream_.clear();
}


Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {
    try {
        // Ensure log folder exists
        std::string logDir = "C:\\Logs";
        _mkdir(logDir.c_str());  // ✅ Creates folder if not exists (safe on Windows)

        // Get current time with milliseconds
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) %
            1000;

        std::tm localTime;
        localtime_s(&localTime, &now_time_t);

        char timeBuf[64];
        std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d_%H-%M-%S", &localTime);

        std::ostringstream fileName;
        fileName << logDir << "\\winLW_session_" << timeBuf << "_"
            << std::setfill('0') << std::setw(3) << now_ms.count() << ".txt";

        // Open the session-specific log file
        logFile_.open(fileName.str(), std::ios::app);
        if (!logFile_.is_open()) {
            std::cerr << "[Logger ERROR] Unable to open log file: "
                << fileName.str() << std::endl;
        }
        else {
            //std::cout << "[Logger INFO] Logging to file: " << fileName.str() << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[Logger ERROR] Exception during initialization: " << e.what() << std::endl;
    }
}



Logger::~Logger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

void Logger::write(LogLevel level,
    const std::string& message,
    const char* file,
    const char* function,
    int line) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Get timestamp
    std::time_t now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now);

    char timeBuf[64];
    std::strftime(timeBuf, sizeof(timeBuf), "%Y/%m/%d %H:%M:%S", &localTime);

    // Thread ID
    auto threadId = std::this_thread::get_id();
    std::ostringstream tidStream;
    tidStream << threadId;
    std::string tid = tidStream.str();

    // Extract file name only
    std::string fileName = file;
    size_t pos = fileName.find_last_of("\\/");
    if (pos != std::string::npos)
        fileName = fileName.substr(pos + 1);

    // Format
    std::ostringstream logLine;
    logLine << timeBuf << " " << tid << "\t"
        << std::left << std::setw(5) << getLevelString(level) << " "
        << "[" << fileName << "->" << function << ":" << line << "]\t"
        << message;

    // Output
    std::string out = logLine.str();

    if (logFile_.is_open()) {
        logFile_ << out << std::endl;
        logFile_.flush();
    }

    //std::cout << out << std::endl;
}
