
#include "afrm/validation.h"

#include <iostream>

int main()
{
    const afrm::FlightInput flight{
        "AFRM-001",
        200.0,
        200.0,
        200.0,
        500.0,
        20.0
    };

    if (const auto error = afrm::validateFlightInput(flight)) {
        std::cerr << "INVALID: " << *error << '\n';
        return 1;
    }

    std::cout << "Flight " << flight.flightId
        << ": inputs valid\n";

    std::cout << "Fuel calculations are not implemented yet\n";

    return 0;
}
