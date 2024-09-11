#include <iostream>
#include <typeinfo>
#include <climits>
#include <cfloat>

int main() {
    // Whole numbers
    int i;
    std::cout << "Name: " << typeid(i).name() << std::endl;
    std::cout << "Size: " << sizeof(i) << " bytes" << std::endl;
    std::cout << "Minimum value: " << INT_MIN << std::endl;
    std::cout << "Maximum value: " << INT_MAX << std::endl;

    unsigned int ui;
    std::cout << "Name: " << typeid(ui).name() << std::endl;
    std::cout << "Size: " << sizeof(ui) << " bytes" << std::endl;
    std::cout << "Minimum value: 0" << std::endl;
    std::cout << "Maximum value: " << UINT_MAX << std::endl;

    short s;
    std::cout << "Name: " << typeid(s).name() << std::endl;
    std::cout << "Size: " << sizeof(s) << " bytes" << std::endl;
    std::cout << "Minimum value: " << SHRT_MIN << std::endl;
    std::cout << "Maximum value: " << SHRT_MAX << std::endl;

    unsigned short us;
    std::cout << "Name: " << typeid(us).name() << std::endl;
    std::cout << "Size: " << sizeof(us) << " bytes" << std::endl;
    std::cout << "Minimum value: 0" << std::endl;
    std::cout << "Maximum value: " << USHRT_MAX << std::endl;

    long l;
    std::cout << "Name: " << typeid(l).name() << std::endl;
    std::cout << "Size: " << sizeof(l) << " bytes" << std::endl;
    std::cout << "Minimum value: " << LONG_MIN << std::endl;
    std::cout << "Maximum value: " << LONG_MAX << std::endl;

    unsigned long ul;
    std::cout << "Name: " << typeid(ul).name() << std::endl;
    std::cout << "Size: " << sizeof(ul) << " bytes" << std::endl;
    std::cout << "Minimum value: 0" << std::endl;
    std::cout << "Maximum value: " << ULONG_MAX << std::endl;

    long long ll;
    std::cout << "Name: " << typeid(ll).name() << std::endl;
    std::cout << "Size: " << sizeof(ll) << " bytes" << std::endl;
    std::cout << "Minimum value: " << LLONG_MIN << std::endl;
    std::cout << "Maximum value: " << LLONG_MAX << std::endl;

    unsigned long long ull;
    std::cout << "Name: " << typeid(ull).name() << std::endl;
    std::cout << "Size: " << sizeof(ull) << " bytes" << std::endl;
    std::cout << "Minimum value: 0" << std::endl;
    std::cout << "Maximum value: " << ULLONG_MAX << std::endl;

    // Boolean
    bool b;
    std::cout << "Name: " << typeid(b).name() << std::endl;
    std::cout << "Size: " << sizeof(b) << " bytes" << std::endl;
    std::cout << "Values: true or false" << std::endl;

    // Symbols
    char c;
    std::cout << "Name: " << typeid(c).name() << std::endl;
    std::cout << "Size: " << sizeof(c) << " bytes" << std::endl;
    std::cout << "Minimum value: " << (int)CHAR_MIN << std::endl;
    std::cout << "Maximum value: " << (int)CHAR_MAX << std::endl;

    unsigned char uc;
    std::cout << "Name: " << typeid(uc).name() << std::endl;
    std::cout << "Size: " << sizeof(uc) << " bytes" << std::endl;
    std::cout << "Minimum value: 0" << std::endl;
    std::cout << "Maximum value: " << (int)UCHAR_MAX << std::endl;

    wchar_t wc = L'a';
    std::cout << "Name: " << typeid(wc).name() << std::endl;
    std::cout << "Size: " << sizeof(wc) << " bytes" << std::endl;
    std::cout << "Example value: " << wc << std::endl;

    // Floating point numbers
    float f;
    std::cout << "Name: " << typeid(f).name() << std::endl;
    std::cout << "Size: " << sizeof(f) << " bytes" << std::endl;
    std::cout << "Minimum value: " << FLT_MIN << std::endl;
    std::cout << "Maximum value: " << FLT_MAX << std::endl;

    double d;
    std::cout << "Name: " << typeid(d).name() << std::endl;
    std::cout << "Size: " << sizeof(d) << " bytes" << std::endl;
    std::cout << "Minimum value: " << DBL_MIN << std::endl;
    std::cout << "Maximum value: " << DBL_MAX << std::endl;

    // Strings
    const char* str = "Hello, World!";
    std::cout << "Name: " << typeid(str).name() << std::endl;
    std::cout << "Size: " << sizeof(str) << " bytes" << std::endl;
    std::cout << "Value: " << str << std::endl;

    return 0;
}

