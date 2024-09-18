#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <random>

// RAII class for managing a semaphore
class Semaphore {
public:
    Semaphore(int initialCount, int maxCount) {
        hSemaphore = CreateSemaphore(NULL, initialCount, maxCount, NULL);
        if (hSemaphore == NULL) {
            throw std::runtime_error("Error creating semaphore.");
        }
    }
    // Destructor
    ~Semaphore() {
        if (hSemaphore) {
            CloseHandle(hSemaphore);
        }
    }

    void wait() {
        WaitForSingleObject(hSemaphore, INFINITE);
    }
    // Signal that resource is available
    void signal() {
        ReleaseSemaphore(hSemaphore, 1, NULL);
    }

private:
    HANDLE hSemaphore;
};

// Struct to hold the parameters that needed to pass to each thread
struct ThreadParams {
    int threadId;
    Semaphore* semaphore;
};


// Function to generate random sleep durations (1 - 3 seconds)
int random_sleep_duration() {
    static std::random_device rd; // Generate non-deterministic random numbers
    static std::mt19937 gen(rd()); // Mersenne Twister pseudo-random number generator
    std::uniform_int_distribution<> dist(1000, 3000); // Generates random integers uniformly distributed over a specified range
    return dist(gen);
}

// Thread function
DWORD WINAPI threadFunction(LPVOID param) {
    ThreadParams* params = static_cast<ThreadParams*>(param);

    // Sleep before opening the file
    Sleep(random_sleep_duration());

    // Wait for access to the file
    params->semaphore->wait();

    // Open the file and write the thread ID and timestamp
    std::ofstream file("output.txt", std::ios_base::app);
    if (file.is_open()) {
        time_t now = time(0); // Get current time as time_t
        char dt[26]; // Buffer to hold the time string (ctime_s requires at least 26 characters)
        errno_t err = ctime_s(dt, sizeof(dt), &now);  // Use ctime_s safely, passing buffer, size, and time
        if (err == 0) {  // Check if ctime_s succeeded
            file << "Thread ID: " << params->threadId << ", Time: " << dt << std::endl;
        }
        else {
            std::cerr << "Error getting time with ctime_s" << std::endl;
        }
        file.close();
    }

    // Sleep after writing to the file
    Sleep(random_sleep_duration());

    // Signal that the file is closed, allowing another thread to proceed
    params->semaphore->signal();

    return 0;
}

int main() {
    int threadCount, maxOpenFiles;

    std::cout << "Enter number of threads: ";
    std::cin >> threadCount;
    std::cout << "Enter maximum number of open files: ";
    std::cin >> maxOpenFiles;

    // Initialize the semaphore with maxOpenFiles permits
    // All available slots to be available initially  (initialCount = maxOpenFiles) and 
    // Max number of concurrent open files to be limited to maxOpenFiles at all times (maxCount = maxOpenFiles)
    Semaphore fileAccessSemaphore(maxOpenFiles, maxOpenFiles);

    // Container to store thread handles
    std::vector<HANDLE> threads(threadCount); // Stores the handles for the threads, allowing the program to manage and control the threads
    std::vector<ThreadParams> threadParams(threadCount); // Stores the parameters (ID, semaphore) for each thread, allowing each thread to have its own unique set of parameters when it runs

    // Create threads
    for (int i = 0; i < threadCount; ++i) {
        threadParams[i].threadId = i + 1; // Set thread ID
        threadParams[i].semaphore = &fileAccessSemaphore; // Set semaphore reference

        threads[i] = CreateThread(
            NULL, 0,
            threadFunction,
            &threadParams[i],
            0, NULL);
    }

    // Wait for all threads to finish
    WaitForMultipleObjects(static_cast<DWORD>(threads.size()), threads.data(), TRUE, INFINITE);

    // Close thread handles
    for (auto& thread : threads) {
        CloseHandle(thread);
    }

    std::cout << "All threads have finished." << std::endl;

    return 0;
}
