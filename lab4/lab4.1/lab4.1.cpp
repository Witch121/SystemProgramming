#include <windows.h>
#include <iostream>

int main() {
    // Create the named (unique) mutex, to all the examples of program can use the same one
    HANDLE hMutex = CreateMutex(NULL, FALSE, L"Lab4Mutex");

    // Check, if mutex already exist or was just created
    if (hMutex == NULL) {
        std::cerr << "Error creating mutex." << std::endl;
        return 1;  // Error by creation mutex - end of the program
    }

    // Check, if programm example already exist
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        std::cerr << "Another instance of the program is already running!" << std::endl;
        CloseHandle(hMutex);  // Close the mutex
        return 1;  // End the program - one already exist
    }

    // if mutex was created, but not opened - show the message 
    std::cout << "Is Running" << std::endl;

    // Wait the program to end postponed
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    // Close the mutex brfor program`s end
    CloseHandle(hMutex);
    return 0;
}
