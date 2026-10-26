
#include "afrm/csv_loader.h"
#include "afrm/fuel_calculator.h"
#include "afrm/fuel_status.h"

#include <fstream>
#include <iomanip>
#include <iostream>

int main(int argc, char* argv[])
{
    // Require one CSV file path as a command-line argument.
    if (argc != 2) {
        std::cerr << "Usage: afrm <flight_csv_path>\n";
        return 1;
    }

    // Open the supplied file.
    std::ifstream input(argv[1]);

    if (!input.is_open()) {
        std::cerr << "ERROR: Cannot open CSV file: "
            << argv[1] << '\n';
        return 1;
    }

    const auto dataset = afrm::loadFlightsCsv(input);

    if (input.bad()) {
        std::cerr << "ERROR: Failed while reading CSV file\n";
        return 1;
    }

    // Reject the dataset if any record is invalid.
    if (!dataset.errors.empty()) {
        std::cerr << "ERROR: CSV validation failed\n";

        for (const auto& error : dataset.errors) {
            std::cerr << "Line " << error.lineNumber
                << ": " << error.message << '\n';
        }

        return 1;
    }

    const afrm::FuelPolicy policy{};

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "=== AFRM PROTOTYPE ===\n\n";

    // Process every validated flight.
    for (const auto& flight : dataset.flights) {
        afrm::FuelResults results{};

        if (const auto error =
            afrm::calculateFuel(flight, policy, results)) {
            std::cerr << "INVALID: Flight "
                << flight.flightId << ": "
                << *error << '\n';
            return 1;
        }

        const auto status = afrm::classifyFuelStatus(
            results.fuelMarginKg,
            policy.cautionThresholdKg
        );

        if (!status) {
            std::cerr << "INVALID: Cannot classify flight "
                << flight.flightId << '\n';
            return 1;
        }

        std::cout << "Flight: " << flight.flightId << '\n';

        std::cout << "  Duration: "
            << results.durationHours << " hours\n";

        std::cout << "  Trip fuel: "
            << results.tripFuelKg << " kg\n";

        std::cout << "  Contingency: "
            << results.contingencyFuelKg << " kg\n";

        std::cout << "  Required fuel: "
            << results.totalRequiredFuelKg << " kg\n";

        std::cout << "  Destination balance: "
            << results.destinationBalanceKg << " kg\n";

        std::cout << "  Fuel margin: "
            << results.fuelMarginKg << " kg\n";

        std::cout << "  Planning range: "
            << results.theoreticalRangeNm << " NM\n";

        std::cout << "  Status: "
            << afrm::fuelStatusToString(*status)
            << "\n\n";
    }

    std::cout << "Processed "
        << dataset.flights.size()
        << " flights\n";

    return 0;
}
