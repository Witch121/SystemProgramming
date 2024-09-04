#include <iostream>
#include <climits>

int main() {
    // whole numbers
    std::cout << "Signed int: Size = " << sizeof(int) << " bytes, Min = 0x"
        << std::hex << INT_MIN << ", Max = 0x" << INT_MAX << std::dec << std::endl;

    std::cout << "Unsigned int: Size = " << sizeof(unsigned int) << " bytes, Min = 0x0, Max = 0x"
        << std::hex << UINT_MAX << std::dec << std::endl;

    std::cout << "Signed short: Size = " << sizeof(short) << " bytes, Min = 0x"
        << std::hex << SHRT_MIN << ", Max = 0x" << SHRT_MAX << std::dec << std::endl;

    std::cout << "Unsigned short: Size = " << sizeof(unsigned short) << " bytes, Min = 0x0, Max = 0x"
        << std::hex << USHRT_MAX << std::dec << std::endl;

    std::cout << "Signed long: Size = " << sizeof(long) << " bytes, Min = 0x"
        << std::hex << LONG_MIN << ", Max = 0x" << LONG_MAX << std::dec << std::endl;

    std::cout << "Unsigned long: Size = " << sizeof(unsigned long) << " bytes, Min = 0x0, Max = 0x"
        << std::hex << ULONG_MAX << std::dec << std::endl;

    std::cout << "Signed long long: Size = " << sizeof(long long) << " bytes, Min = 0x"
        << std::hex << LLONG_MIN << ", Max = 0x" << LLONG_MAX << std::dec << std::endl;

    std::cout << "Unsigned long long: Size = " << sizeof(unsigned long long) << " bytes, Min = 0x0, Max = 0x"
        << std::hex << ULLONG_MAX << std::dec << std::endl;

    // boolean
    std::cout << "Boolean: Size = " << sizeof(bool) << " bytes, Values = true or false" << std::endl;

    // symbols
    std::cout << "Char: Size = " << sizeof(char) << " bytes, Min = 0x"
        << std::hex << (int)CHAR_MIN << ", Max = 0x" << (int)CHAR_MAX << std::dec << std::endl;

    std::cout << "Unsigned char: Size = " << sizeof(unsigned char) << " bytes, Min = 0x0, Max = 0x"
        << std::hex << (int)UCHAR_MAX << std::dec << std::endl;

    wchar_t wc = L'a';
    std::cout << "Wide char (wchar_t): Size = " << sizeof(wchar_t) << " bytes, Example value = " << wc << std::endl;

    // strings
    const char* str = "Hello, World!";
    std::cout << "Pointer to string (const char*): Size = " << sizeof(str) << " bytes, Value = " << str << std::endl;

    return 0;
}

