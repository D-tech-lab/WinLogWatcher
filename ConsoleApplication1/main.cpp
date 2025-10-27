#include "logWatcher.h"
#include "logger.h"
#include <vector>
#include <string>
#include <mutex>
using namespace std;
int main() {
    log(INFO) << "WinLogWatch started." << endl;

    std::string filePath = "C:\\Logs\\app_log.txt";
    std::vector<std::string> patterns = {"ERROR", "CRITICAL", "FAIL", "EXCEPTION"};

    LogWatcher watcher(filePath, patterns);
    watcher.startMonitoring();

    log(INFO) << "WinLogWatch terminated." << endl;
    return 0;
}
