#ifndef LOG_WATCHER_H
#define LOG_WATCHER_H

#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <mutex>
#include "logger.h"
#include <iostream>
#include <ctime>
#include <thread>

using namespace std;
/**
 * @brief The LogWatcher class monitors a specified log file for specific keyword patterns.
 * When a matching pattern is found, it triggers an alert (e.g., calls a PowerShell script).
 * 
 * Responsibilities:
 *  - Continuously read new lines appended to a log file
 *  - Match lines against configurable patterns (regex)
 *  - Log all actions and alerts using the Singleton Logger
 *  - Execute an external alert script when a match occurs
 */
class LogWatcher {
public:
    /**
     * @brief Construct a new LogWatcher object
     * 
     * @param filePath Path to the log file being monitored
     * @param patterns List of string patterns or regex expressions to detect
     */
    LogWatcher(const std::string& filePath, const std::vector<std::string>& patterns);

    /**
     * @brief Starts continuous monitoring of the specified log file.
     * This function runs an infinite loop (polling every few seconds)
     * until the program is stopped manually.
     */
    void startMonitoring();

private:
    std::string filePath;               ///< Path to the log file being watched
    std::vector<std::string> patterns;  ///< Patterns to detect in log lines
    std::streampos lastReadPos;         ///< Position in file after last read

    /**
     * @brief Reads and processes new lines added to the log file since the last check.
     */
    void checkNewLines();

    /**
     * @brief Matches a line against all configured patterns.
     * 
     * @param line Current line read from log file
     * @param matchedPattern Reference to store the matched pattern string
     * @return true if a match is found, false otherwise
     */
    bool matchPattern(const std::string& line, std::string& matchedPattern);

    /**
     * @brief Triggers an alert by executing the PowerShell script with the pattern and line as arguments.
     * 
     * @param pattern The matched pattern that caused the alert
     * @param line The actual log line containing the pattern
     */
    void triggerAlert(const std::string& pattern, const std::string& line);
};

#endif // LOG_WATCHER_H
