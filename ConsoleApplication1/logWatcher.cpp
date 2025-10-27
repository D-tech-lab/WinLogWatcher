
#include <iostream>
#include <ctime>
#include <mutex>
#include <thread>
#include "logWatcher.h"
#include "logger.h"
#include <thread>
#include <chrono>
#include <fstream>
#include <regex>

using namespace std;

LogWatcher::LogWatcher(const std::string& filePath, const std::vector<std::string>& patterns)
    : filePath(filePath), patterns(patterns), lastReadPos(0)  // initializer list
{
    log(INFO) << "LogWatcher initialized for file: " << filePath << endl;
}

void LogWatcher::startMonitoring() {
    log(INFO) << "Watching file: " << filePath << endl;

    while (true) {
        checkNewLines();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void LogWatcher::checkNewLines() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        log(ERR) << "Failed to open log file: " << filePath << endl;
        return;
    }

    file.seekg(lastReadPos);
    std::string line;
    while (std::getline(file, line)) {
        std::string matchedPattern;
        if (matchPattern(line, matchedPattern)) {
            log(WARN) << "Pattern \"" + matchedPattern + "\" found: " + line << endl;
            triggerAlert(matchedPattern, line);
        }
    }

    lastReadPos = file.tellg();
    file.close();
}

void LogWatcher::triggerAlert(const std::string& pattern, const std::string& line) {
    std::string command = "powershell -ExecutionPolicy Bypass -File \"scripts/alert.ps1\" "
                          "-pattern \"" + pattern + "\" -line \"" + line + "\"";

    log(INFO) << "Triggering PowerShell alert for pattern: " << pattern << endl;
    system(command.c_str());
}
bool LogWatcher::matchPattern(const std::string& line, std::string& matchedPattern) {
    for (const auto& pattern : patterns) {
        try {
            std::regex regexPattern(pattern);
            if (std::regex_search(line, regexPattern)) {
                matchedPattern = pattern;
                return true;
            }
        }
        catch (const std::regex_error& e) {
            log(ERR) << "Invalid regex pattern: " + pattern + " | " + e.what() << endl;
        }
    }
    return false;
}
