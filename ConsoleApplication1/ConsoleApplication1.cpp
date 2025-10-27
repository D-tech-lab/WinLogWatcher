#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex mtx;  // Shared mutex for synchronization

void printMessage(const string& message, int count) {
    for (int i = 0; i < count; ++i) {
        // Lock the mutex before accessing shared output
        lock_guard<mutex> lock(mtx);
        cout << "Thread [" << this_thread::get_id() << "] says: " << message << endl;
    }
}

int main() {
    cout << "=== Gaurav’s Thread + Mutex Test ===" << endl;

    // Create two threads that print messages
    thread t1(printMessage, "Hello from Thread 1", 5);
    thread t2(printMessage, "Hello from Thread 2", 5);

    // Wait for both threads to finish
    t1.join();
    t2.join();

    cout << "=== All threads completed successfully ===" << endl;
    return 0;
}
