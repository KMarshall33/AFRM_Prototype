
#include <iostream>

int main()
{
    const int diagnosticValue = 42;

    std::cout << "AFRM prototype: toolchain check successful\n";
    std::cout << "Debugger check: " << diagnosticValue << '\n';

    return 0;
}
