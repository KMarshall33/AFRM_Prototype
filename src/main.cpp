
#include "afrm/fuel_calculator.h"
#include "afrm/fuel_status.h"

#include <iomanip>
#include <iostream>

int main()
{
    // Simulated flight AFRM-001
    const afrm::FlightInput flight{
        "AFRM-001",
        200.0, // Distance (NM)
        200.0, // Groundspeed (knots)
        200.0, // Fuel burn (kg/hour)
        500.0, // Available fuel (kg)
        20.0   // Taxi fuel (kg)
    };

    // Approved simulated fuel policy
    const afrm::FuelPolicy policy{};

    afrm::FuelResults results{};

    // Calculate flight fuel requirements
    if (const auto error =
        afrm::calculateFuel(flight, policy, results)) {
        std::cerr << "INVALID: " << *error << '\n';
        return 1;
    }

    // Classify the calculated fuel margin
    const auto status = afrm::classifyFuelStatus(
        results.fuelMarginKg,
        policy.cautionThresholdKg
    );

    if (!status) {
        std::cerr << "INVALID: Fuel status cannot be classified\n";
        return 1;
    }

    // Display results
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "=== AFRM PROTOTYPE ===\n";
    std::cout << "Flight: " << flight.flightId << "\n\n";

    std::cout << "Flight duration: "
        << results.durationHours << " hours\n";

    std::cout << "Trip fuel: "
        << results.tripFuelKg << " kg\n";

    std::cout << "Contingency fuel: "
        << results.contingencyFuelKg << " kg\n";

    std::cout << "Total required fuel: "
        << results.totalRequiredFuelKg << " kg\n";

    std::cout << "Destination balance: "
        << results.destinationBalanceKg << " kg\n";

    std::cout << "Remaining fuel margin: "
        << results.fuelMarginKg << " kg\n";

    std::cout << "Theoretical planning range: "
        << results.theoreticalRangeNm << " NM\n";

    std::cout << "\nFuel status: "
        << afrm::fuelStatusToString(*status)
        << '\n';

    return 0;
}
