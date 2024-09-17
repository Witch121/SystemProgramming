#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

bool execute_7z_command(const std::wstring& command) {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    si.dwFlags |= STARTF_USESTDHANDLES;
    HANDLE hStdOut = CreateFile(L"output.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    si.hStdOutput = hStdOut;

    std::wcout << L"Executing command: " << command << std::endl;

    if (!CreateProcess(NULL, const_cast<wchar_t*>(command.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        DWORD errorCode = GetLastError();
        std::cerr << "Failed to execute 7z command. Error code: " << errorCode << std::endl;

        LPVOID lpMsgBuf;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&lpMsgBuf,
            0, NULL);

        std::wcerr << L"Error message: " << (LPWSTR)lpMsgBuf << std::endl;
        LocalFree(lpMsgBuf);

        return false;
    }

    // Wait for the process to complete
    DWORD waitResult = WaitForSingleObject(pi.hProcess, INFINITE);
    if (waitResult == WAIT_FAILED) {
        std::cerr << "Failed to wait for 7z process to complete." << std::endl;
        return false;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hStdOut);

    // Check the contents of output.txt
    std::ifstream outputFile("output.txt");
    if (outputFile.is_open()) {
        std::string line;
        while (std::getline(outputFile, line)) {
            std::cout << line << std::endl;
        }
        outputFile.close();
    } else {
        std::cerr << "Failed to open output.txt" << std::endl;
    }

    return true;
}

void pack_file(const std::wstring& file_name, const std::wstring& archive_name) {
    std::wstringstream command;
    command << L"7z.exe a " << archive_name << L".zip " << file_name;
    if (execute_7z_command(command.str())) {
        std::cout << "File packed successfully." << std::endl;
    } else {
        std::cerr << "Failed to pack file." << std::endl;
    }
}

void unpack_archive(const std::wstring& archive_name, const std::wstring& destination_folder) {
    std::wstringstream command;
    command << L"7z.exe x " << archive_name << L" -o" << destination_folder;
    if (execute_7z_command(command.str())) {
        std::cout << "Archive unpacked successfully." << std::endl;
    } else {
        std::cerr << "Failed to unpack archive." << std::endl;
    }
}

int main() {
    std::string command;
    std::wstring file_name, archive_name, destination_folder;

    while (true) {
        std::cout << "Enter command (pack/unpack/exit): ";
        std::cin >> command;

        if (command == "pack") {
            std::wcout << L"Enter file name: ";
            std::wcin >> file_name;
            std::wcout << L"Enter archive name: ";
            std::wcin >> archive_name;
            pack_file(file_name, archive_name);
        } else if (command == "unpack") {
            std::wcout << L"Enter archive name: ";
            std::wcin >> archive_name;
            std::wcout << L"Enter destination folder: ";
            std::wcin >> destination_folder;
            unpack_archive(archive_name, destination_folder);
        } else if (command == "exit") {
            break;
        } else {
            std::cerr << "Unknown command!" << std::endl;
        }
    }

    return 0;
}
