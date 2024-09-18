#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <limits>
#define NOMINMAX
#include <windows.h>

// Global var
std::vector<int> data; // Array
int sum = 0;
double average = 0.0;
int minValue = std::numeric_limits<int>::max();
int maxValue = std::numeric_limits<int>::min();

// Critical section for mutual exclusion
CRITICAL_SECTION criticalSection;

// Sum
void calculateSum(bool useMutex) {
    int localSum = 0;
    if (useMutex) {
        EnterCriticalSection(&criticalSection); // Entrance to the critical section
    }
    for (int num : data) {
        localSum += num;
    }
    sum = localSum;
    if (useMutex) {
        LeaveCriticalSection(&criticalSection); // Exit from the critical section
    }
}

// Average 
void calculateAverage(bool useMutex) {
    if (useMutex) {
        EnterCriticalSection(&criticalSection);
    }
    if (!data.empty()) {
        average = static_cast<double>(sum) / data.size();
    }
    if (useMutex) {
        LeaveCriticalSection(&criticalSection);
    }
}

// Min and max
void findMinMax(bool useMutex) {
    int localMin = std::numeric_limits<int>::max();
    int localMax = std::numeric_limits<int>::min();
    if (useMutex) {
        EnterCriticalSection(&criticalSection);
    }
    for (int num : data) {
        if (num < localMin) localMin = num;
        if (num > localMax) localMax = num;
    }
    minValue = localMin;
    maxValue = localMax;
    if (useMutex) {
        LeaveCriticalSection(&criticalSection);
    }
}

// Start the threads and record the time
void runProgram(bool useMutex) {
    sum = 0;
    average = 0.0;
    minValue = std::numeric_limits<int>::max();
    maxValue = std::numeric_limits<int>::min();

    // Start the threads
    auto start = std::chrono::high_resolution_clock::now();

    std::thread thread1(calculateSum, useMutex);
    std::thread thread2(calculateAverage, useMutex);
    std::thread thread3(findMinMax, useMutex);

    // Wait till the end of all the processes
    thread1.join();
    thread2.join();
    thread3.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Show thw results
    std::cout << "Execution time: " << duration.count() << " sec" << std::endl;
    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Avverage: " << average << std::endl;
    std::cout << "Min: " << minValue << ", Max: " << maxValue << std::endl;
}

int main() {
    // Initialization of the critical section
    InitializeCriticalSection(&criticalSection);

    // Fill the array with data
    int dataSize = 50000; 
    data.resize(dataSize);
    for (int i = 0; i < dataSize; ++i) {
        data[i] = rand() % 1000; // Fill with random values 0 - 999
    }

    // Non-mutual exclusion mode
    std::cout << "Non-mutal exclusion mode:" << std::endl;
    runProgram(false);

    // Mutal exclusion mode
    std::cout << "Mutal exclusion mode:" << std::endl;
    runProgram(true);

    // Delete the critical section
    DeleteCriticalSection(&criticalSection);

    return 0;
}
