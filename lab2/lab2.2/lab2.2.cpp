#include <windows.h>
#include <iostream>
#include <string>

// Helper to convert std::string to std::wstring for Windows API
std::wstring string_to_wstring(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
    return wstr;
}

// Change directory
void change_directory(const std::string& path) {
    std::wstring wpath = string_to_wstring(path);
    if (SetCurrentDirectory(wpath.c_str())) {
        std::cout << "Changed directory to: " << path << std::endl;
    }
    else {
        std::cerr << "Failed to change directory" << std::endl;
    }
}

// List directory contents
void list_directory() {
    WIN32_FIND_DATA find_data;
    HANDLE hFind = FindFirstFile(L"*", &find_data);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to list directory" << std::endl;
        return;
    }

    do {
        std::cout << (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? "[DIR] " : "[FILE] ")
            << find_data.cFileName << std::endl;
    } while (FindNextFile(hFind, &find_data) != 0);

    FindClose(hFind);
}

// Copy a file
void copy_file(const std::string& source, const std::string& dest) {
    std::wstring wsource = string_to_wstring(source);
    std::wstring wdest = string_to_wstring(dest);
    if (CopyFile(wsource.c_str(), wdest.c_str(), FALSE)) {
        std::cout << "File copied from " << source << " to " << dest << std::endl;
    }
    else {
        std::cerr << "Failed to copy file" << std::endl;
    }
}

// Create a directory
void create_directory(const std::string& dir_name) {
    std::wstring wdir_name = string_to_wstring(dir_name);
    if (CreateDirectory(wdir_name.c_str(), nullptr)) {
        std::cout << "Directory created: " << dir_name << std::endl;
    }
    else {
        std::cerr << "Failed to create directory" << std::endl;
    }
}

// Delete a file or empty directory
void delete_file_or_directory(const std::string& path) {
    std::wstring wpath = string_to_wstring(path);

    DWORD attributes = GetFileAttributes(wpath.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        std::cerr << "File or directory not found: " << path << std::endl;
        return;
    }

    if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
        if (RemoveDirectory(wpath.c_str())) {
            std::cout << "Directory deleted: " << path << std::endl;
        }
        else {
            std::cerr << "Failed to delete directory (it might not be empty): " << path << std::endl;
        }
    }
    else {
        if (DeleteFile(wpath.c_str())) {
            std::cout << "File deleted: " << path << std::endl;
        }
        else {
            std::cerr << "Failed to delete file: " << path << std::endl;
        }
    }
}

// Print detailed file information
void print_file_info(const std::string& path) {
    std::wstring wpath = string_to_wstring(path);
    WIN32_FILE_ATTRIBUTE_DATA file_info;

    if (GetFileAttributesEx(wpath.c_str(), GetFileExInfoStandard, &file_info)) {
        SYSTEMTIME creation_time, last_access_time, last_write_time;
        FileTimeToSystemTime(&file_info.ftCreationTime, &creation_time);
        FileTimeToSystemTime(&file_info.ftLastAccessTime, &last_access_time);
        FileTimeToSystemTime(&file_info.ftLastWriteTime, &last_write_time);

        std::cout << "File: " << path << std::endl;
        std::cout << "Size: " << file_info.nFileSizeLow << " bytes" << std::endl;
        std::cout << "Created: " << creation_time.wDay << "/" << creation_time.wMonth << "/" << creation_time.wYear
            << " " << creation_time.wHour << ":" << creation_time.wMinute << std::endl;
        std::cout << "Last accessed: " << last_access_time.wDay << "/" << last_access_time.wMonth << "/" << last_access_time.wYear
            << " " << last_access_time.wHour << ":" << last_access_time.wMinute << std::endl;
        std::cout << "Last modified: " << last_write_time.wDay << "/" << last_write_time.wMonth << "/" << last_write_time.wYear
            << " " << last_write_time.wHour << ":" << last_write_time.wMinute << std::endl;
    }
    else {
        std::cerr << "Failed to retrieve file information: " << path << std::endl;
    }
}

int main() {
    std::string command, arg1, arg2;

    while (true) {
        std::cout << "Enter command: ";
        std::cin >> command;

        if (command == "cd") {
            std::cin >> arg1;
            change_directory(arg1);
        }
        else if (command == "ls") {
            list_directory();
        }
        else if (command == "cp") {
            std::cin >> arg1 >> arg2;
            copy_file(arg1, arg2);
        }
        else if (command == "mkdir") {
            std::cin >> arg1;
            create_directory(arg1);
        }
        else if (command == "del") {
            std::cin >> arg1;
            delete_file_or_directory(arg1);
        }
        else if (command == "info") {
            std::cin >> arg1;
            print_file_info(arg1);
        }
        else if (command == "exit") {
            break;
        }
        else {
            std::cerr << "Unknown command" << std::endl;
        }
    }

    return 0;
}
