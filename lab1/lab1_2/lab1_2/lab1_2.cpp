#include <iostream>
#include <windows.h> // Windows API
#include <cstdio> //standart C lib for files
#include <vector> // dinamic array

void ConvertToUnicode(const char* inputFile, const char* outputFile) {
    FILE* input = nullptr;
    FILE* output = nullptr;

    errno_t err = fopen_s(&input, inputFile, "rb"); // read binary
    if (err != 0) {
        std::cerr << "Error opening input file!" << std::endl;
        return;
    }

    err = fopen_s(&output, outputFile, "wb"); // write binary
    if (err != 0) {
        std::cerr << "Error opening output file!" << std::endl;
        if (input) fclose(input);
        return;
    }


    if (!input || !output) { // check files
        std::cerr << "Error opening files!" << std::endl;
        if (input) fclose(input);
        if (output) fclose(output);
        return;
    }

    fseek(input, 0, SEEK_END); // pointer to the end - check the size
    size_t fileSize = ftell(input); // check pointer > in the end > size in bytes
    fseek(input, 0, SEEK_SET); // pointer to start

    std::vector<char> buffer(fileSize); // create the new dinamic array
    fread(buffer.data(), 1, fileSize, input); // read the data from input file, size - for each element 1byte, pointer to the start 

    int wideCharSize = MultiByteToWideChar(CP_ACP, 0, buffer.data(), fileSize, NULL, 0); // ASCI > Unicode, check the size of file
    std::vector<wchar_t> wideBuffer(wideCharSize); // create the dinamic array for storage the Unicode
    MultiByteToWideChar(CP_ACP, 0, buffer.data(), fileSize, wideBuffer.data(), wideCharSize); // convert the text

    fwrite(wideBuffer.data(), sizeof(wchar_t), wideBuffer.size(), output); // write to the outout file in Unicode, with size of each element as wchar_t

    fclose(input);
    fclose(output);
}

void ConvertToANSI(const char* inputFile, const char* outputFile) {
    FILE* input = nullptr;
    FILE* output = nullptr;

    errno_t err = fopen_s(&input, inputFile, "rb"); // read binary
    if (err != 0) {
        std::cerr << "Error opening input file!" << std::endl;
        return;
    }

    err = fopen_s(&output, outputFile, "wb"); // write binary
    if (err != 0) {
        std::cerr << "Error opening output file!" << std::endl;
        if (input) fclose(input);
        return;
    }

    if (!input || !output) { // check 
        std::cerr << "Error opening files!" << std::endl;
        if (input) fclose(input);
        if (output) fclose(output);
        return;
    }

    fseek(input, 0, SEEK_END);
    size_t fileSize = ftell(input);
    fseek(input, 0, SEEK_SET);

    std::vector<wchar_t> wideBuffer(fileSize / sizeof(wchar_t)); // create the vector of input file in Unicode
    fread(wideBuffer.data(), sizeof(wchar_t), wideBuffer.size(), input); // read the data from input to the vector

    int ansiCharSize = WideCharToMultiByte(CP_ACP, 0, wideBuffer.data(), wideBuffer.size(), NULL, 0, NULL, NULL); // Unicode > ASCI, check the requared size
    std::vector<char> ansiBuffer(ansiCharSize); // create the vector
    WideCharToMultiByte(CP_ACP, 0, wideBuffer.data(), wideBuffer.size(), ansiBuffer.data(), ansiCharSize, NULL, NULL); // convert with requared size

    fwrite(ansiBuffer.data(), 1, ansiBuffer.size(), output); // write to output file from vector with size of each element 1 

    fclose(input);
    fclose(output);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " -a|-u inputfile outputfile" << std::endl;
        return 1;
    }

    const char* mode = argv[1]; 
    const char* inputFile = argv[2]; // name of input
    const char* outputFile = argv[3]; // name of output

    if (strcmp(mode, "-a") == 0) {
        ConvertToUnicode(inputFile, outputFile);
    }
    else if (strcmp(mode, "-u") == 0) {
        ConvertToANSI(inputFile, outputFile);
    }
    else {
        std::cerr << "Invalid argument. Use -a for ASCII to Unicode conversion or -u for Unicode to ASCII conversion." << std::endl;
        return 1;
    }

    return 0;
}
