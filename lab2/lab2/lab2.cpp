#include <windows.h>
#include <iostream>
#include <string>
#include <iomanip>

struct Record {
    int record_number;
    FILETIME creation_time;
    wchar_t text[80];
    int modify_count;
};

struct FileHeader {
    int non_empty_records;
    DWORD file_size;
};

// Create a new record
void create_record(HANDLE file, int record_number) {
    // Initialize a new record
    Record record = {};
    record.record_number = record_number;

    GetSystemTimeAsFileTime(&record.creation_time);

    // Input text
    std::wcout << L"Input your text (up to 80 symbols): ";
    std::wstring input_text;
    std::getline(std::wcin, input_text);

    // Copy the text
    wcsncpy_s(record.text, input_text.c_str(), _countof(record.text));

    if (input_text.length() < _countof(record.text)) {
        std::fill(record.text + input_text.length(), record.text + 80, L' ');
    }
    record.modify_count = 0;

    // Move file pointer to the correct position
    SetFilePointer(file, sizeof(FileHeader) + sizeof(Record) * record_number, nullptr, FILE_BEGIN);

    // Write the new record to the file
    DWORD written;
    WriteFile(file, &record, sizeof(Record), &written, nullptr);
}

// Convert FILETIME to readable format
std::string filetime_to_string(const FILETIME& ft) {
    SYSTEMTIME st;
    FileTimeToSystemTime(&ft, &st);

    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%02d/%02d/%04d %02d:%02d:%02d",
        st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

    return std::string(buffer);
}

// Read from file
void read_record(HANDLE file, int record_number) {
    Record record;

    // Move file pointer to the correct position (considering FileHeader size)
    SetFilePointer(file, sizeof(FileHeader) + sizeof(Record) * record_number, nullptr, FILE_BEGIN);

    DWORD read;
    if (!ReadFile(file, &record, sizeof(record), &read, nullptr) || read != sizeof(record)) {
        std::cerr << "Error reading the file!" << std::endl;
        return;
    }

    // Display
    std::cout << "Record number: " << record.record_number << std::endl;
    std::cout << "Creation time: " << filetime_to_string(record.creation_time) << std::endl;
    std::wcout << L"Text: " << record.text << std::endl;
    std::cout << "Modify count: " << record.modify_count << std::endl;
}

int main() {
    // Open the file for reading and writing
    HANDLE file = CreateFile(L"records.dat", GENERIC_WRITE | GENERIC_READ, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to open file" << std::endl;
        return 1;
    }

    // Example: create a new record with some text
    create_record(file, 1);

    // Read and display the record
    read_record(file, 1);

    CloseHandle(file);
    return 0;
}
