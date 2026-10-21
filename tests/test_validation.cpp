
#include "afrm/validation.h"

#include <iostream>
#include <limits>
#include <string_view>

int main()
{
    const afrm::FlightInput baseline{
        "AFRM-001", 200.0, 200.0, 200.0, 500.0, 20.0
    };

    int failures = 0;

    auto check = [&](std::string_view name,
        const afrm::FlightInput& input,
        bool expectedValid) {
            const auto error = afrm::validateFlightInput(input);
            const bool actualValid = !error.has_value();

            std::cout << (actualValid == expectedValid ? "PASS: " : "FAIL: ")
                << name;

            if (error) {
                std::cout << " (" << *error << ')';
            }

            std::cout << '\n';

            if (actualValid != expectedValid) {
                ++failures;
            }
        };

    check("Normal flight", baseline, true);

    auto flight = baseline;
    flight.flightId.clear();
    check("Missing identifier", flight, false);

    flight = baseline;
    flight.distanceNm = -1.0;
    check("Negative distance", flight, false);

    flight = baseline;
    flight.distanceNm = 0.0;
    check("Zero distance", flight, true);

    flight = baseline;
    flight.groundSpeedKnots = 0.0;
    check("Zero groundspeed", flight, false);

    flight = baseline;
    flight.fuelBurnKgPerHour = 0.0;
    check("Zero burn rate", flight, false);

    flight = baseline;
    flight.availableFuelKg = -1.0;
    check("Negative available fuel", flight, false);

    flight = baseline;
    flight.taxiFuelKg = -1.0;
    check("Negative taxi fuel", flight, false);

    flight = baseline;
    flight.taxiFuelKg = 501.0;
    check("Taxi fuel exceeds available", flight, false);

    flight = baseline;
    flight.availableFuelKg = 0.0;
    flight.taxiFuelKg = 0.0;
    check("Zero usable fuel", flight, true);

    flight = baseline;
    flight.distanceNm = std::numeric_limits<double>::infinity();
    check("Infinite distance", flight, false);

    flight = baseline;
    flight.fuelBurnKgPerHour = std::numeric_limits<double>::quiet_NaN();
    check("NaN burn rate", flight, false);

    std::cout << "Validation failures: " << failures << '\n';

    return failures == 0 ? 0 : 1;
}
