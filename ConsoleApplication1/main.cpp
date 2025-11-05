#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <iomanip>

#pragma comment(lib, "Ws2_32.lib")

#include "logWatcher.h"
#include "logger.h"
#include <vector>
#include <string>
#include <mutex>
using namespace std;
int main() {
    
    string banner = R"( 
                        ╱$$      ╱$$ ╱$$           ╱$$                           ╱$$      ╱$$             ╱$$               ╱$$                          
                       │ $$  ╱$ │ $$│__╱          │ $$                          │ $$  ╱$ │ $$            │ $$              │ $$                          
                       │ $$ ╱$$$│ $$ ╱$$ ╱$$$$$$$ │ $$        ╱$$$$$$   ╱$$$$$$ │ $$ ╱$$$│ $$  ╱$$$$$$  ╱$$$$$$    ╱$$$$$$$│ $$$$$$$   ╱$$$$$$   ╱$$$$$$ 
                       │ $$╱$$ $$ $$│ $$│ $$__  $$│ $$       ╱$$__  $$ ╱$$__  $$│ $$╱$$ $$ $$ │____  $$│_  $$_╱   ╱$$_____╱│ $$__  $$ ╱$$__  $$ ╱$$__  $$
                       │ $$$$_  $$$$│ $$│ $$  ╲ $$│ $$      │ $$  ╲ $$│ $$  ╲ $$│ $$$$_  $$$$  ╱$$$$$$$  │ $$    │ $$      │ $$  ╲ $$│ $$$$$$$$│ $$  ╲__╱
                       │ $$$╱ ╲  $$$│ $$│ $$  │ $$│ $$      │ $$  │ $$│ $$  │ $$│ $$$╱ ╲  $$$ ╱$$__  $$  │ $$ ╱$$│ $$      │ $$  │ $$│ $$_____╱│ $$      
                       │ $$╱   ╲  $$│ $$│ $$  │ $$│ $$$$$$$$│  $$$$$$╱│  $$$$$$$│ $$╱   ╲  $$│  $$$$$$$  │  $$$$╱│  $$$$$$$│ $$  │ $$│  $$$$$$$│ $$      
                       │__╱     ╲__╱│__╱│__╱  │__╱│________╱ ╲______╱  ╲____  $$│__╱     ╲__╱ ╲_______╱   ╲___╱   ╲_______╱│__╱  │__╱ ╲_______╱│__╱      
                                                                       ╱$$  ╲ $$                                                                         
                                                                       │  $$$$$$╱                                                                         
                                                                       ╲______╱                                                                          )";
    log(INFO) << endl<< banner << endl;
    log(INFO) << "WinLogWatch started." << endl;
    ofstream file;

    // Opening file "Gfg.txt" in write mode.
    file.open("C:\\Logs\\app_log.txt");
    log(INFO) << "WinLogWatcher is developed by Aayush Daghottra" << endl;
    log(INFO) << "LinkedIn: https://www.linkedin.com/in/aayush-daghottra-12b96417a/ " << endl;

    // Check if the file was successfully created.
    if (!file.is_open())
    {
        log(INFO) << "Error in creating file!" << endl;

        // Return a non-zero value to indicate an error.
        return 1;
    }
    log(INFO) << "File created successfully." << endl;

    // Close the file to free up resources.
    file.close();
    std::string filePath = "C:\\Logs\\app_log.txt";
    std::vector<std::string> patterns = { "ERROR", "CRITICAL", "FAIL", "EXCEPTION" };

    LogWatcher watcher(filePath, patterns);
    watcher.startMonitoring();

    log(INFO) << "WinLogWatch terminated." << endl;
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return 1;
    }
    return 0;
}