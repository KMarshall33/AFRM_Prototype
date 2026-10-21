
#include <array>
#include <iostream>
#include <span>

int main()
{
    // Verify that a C++20 test executable can run.
    const std::array<int, 3> values{ 10, 20, 30 };
    const std::span<const int> view{ values };

    int total = 0;

    for (const int value : view)
    {
        total += value;
    }

    if (total != 60)
    {
        std::cerr << "Test setup failed\n";
        return 1;
    }

    std::cout << "CTest setup passed: C++20 test executable runs\n";
    return 0;
}
