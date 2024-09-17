#include <windows.h>
#include <iostream>
#include <vector>
#include <thread>

__declspec(thread) int tls_sum = 0;
HANDLE console_mutex;  // Mutex for output synchronization

// Function to calculate the largest divisor for a number
int calculate_largest_divisor(int number) {
    for (int i = number / 2; i > 0; --i) {
        if (number % i == 0) {
            return i;
        }
    }
    return 1; // If no divisor found
}

// Thread function to process the array and calculate sum of divisors
DWORD WINAPI thread_function(LPVOID param) {
    std::vector<int>* numbers = static_cast<std::vector<int>*>(param); // Array of numbers

    for (int number : *numbers) {
        int divisor = calculate_largest_divisor(number);
        tls_sum += divisor; // Add a divisor to the total amount stored in TLS
    }

    // Locking the mutex before outputting
    WaitForSingleObject(console_mutex, INFINITE);
    std::cout << "Thread ID: " << GetCurrentThreadId() << ", Sum of divisors: " << tls_sum << std::endl;
    ReleaseMutex(console_mutex);
    return 0;
}

int main() {
    // Create the mutex
    console_mutex = CreateMutex(NULL, FALSE, NULL);

    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Initialize random number generator

    int thread_count;
    std::cout << "Enter number of threads: ";
    std::cin >> thread_count;

    HANDLE* threads = new HANDLE[thread_count]; // Save the threads
    std::vector<std::vector<int>> thread_numbers(thread_count);  // Separate array for each thread

    for (int i = 0; i < thread_count; ++i) {
        int array_size = 5 + std::rand() % 6;  // Array 5 - 10 elements
        for (int j = 0; j < array_size; ++j) {
            thread_numbers[i].push_back(10 + std::rand() % 91);  // Numbers 10 - 100
        }
    }

    //Create the threads
    for (int i = 0; i < thread_count; ++i) {
        threads[i] = CreateThread(NULL, 0, thread_function, &thread_numbers[i], 0, NULL);
    }

    WaitForMultipleObjects(thread_count, threads, TRUE, INFINITE);

    for (int i = 0; i < thread_count; ++i) {
        CloseHandle(threads[i]);
    }

    delete[] threads;
    return 0;
}
