#include <iostream>
#include <windows.h> //lib for use Windows functions, as system info

void GenerateError() {
    // allocation of memory in the local memory, with predicted incorrect args -1
    LocalAlloc(LMEM_FIXED, -1);

    // last error code
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID != 0) {
        LPSTR messageBuffer = nullptr;

        // describing of the error (allocate memory, message error from system, ignore any possible inserts in message, language according to the system, write in buffer)
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        std::cerr << "Error: " << messageBuffer << std::endl;

        // allocate of memory for the message error
        LocalFree(messageBuffer);
    }
    else {
        std::cout << "No error generated." << std::endl;
    }
}

void PrintSystemInfo() {
    //get the different info from Windows
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    std::cout << "Hardware info: " << std::endl;
    std::cout << "  OEM ID: " << si.dwOemId << std::endl;
    std::cout << "  Number of processors: " << si.dwNumberOfProcessors << std::endl;
    std::cout << "  Page size: " << si.dwPageSize << " bytes" << std::endl;
    std::cout << "  Processor type: " << si.dwProcessorType << std::endl;
    std::cout << "  Active processor mask: " << si.dwActiveProcessorMask << std::endl;

    MEMORYSTATUS memStatus;
    GlobalMemoryStatus(&memStatus);

    std::cout << "\nMemory Status: " << std::endl;
    std::cout << "  Total physical memory: " << memStatus.dwTotalPhys / (1024 * 1024) << " MB" << std::endl;
    std::cout << "  Available physical memory: " << memStatus.dwAvailPhys / (1024 * 1024) << " MB" << std::endl;
    std::cout << "  Total virtual memory: " << memStatus.dwTotalVirtual / (1024 * 1024) << " MB" << std::endl;
    std::cout << "  Available virtual memory: " << memStatus.dwAvailVirtual / (1024 * 1024) << " MB" << std::endl;
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " -e|-s" << std::endl;
        return 1;
    }

    if (strcmp(argv[1], "-e") == 0) {
        GenerateError();
    }
    else if (strcmp(argv[1], "-s") == 0) {
        PrintSystemInfo();
    }
    else {
        std::cerr << "Invalid argument. Use -e for error generation or -s for system information." << std::endl;
        return 1;
    }

    return 0;
}
