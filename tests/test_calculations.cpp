
#include "afrm/fuel_calculator.h"

#include <cmath>
#include <iostream>
#include <limits>
#include <string_view>

int main()
{
    int failures = 0;

    const afrm::FlightInput baseline{
        "AFRM-001", 200.0, 200.0, 200.0, 500.0, 20.0
    };

    const afrm::FuelPolicy policy{};

    auto check = [&](std::string_view name,
        double actual,
        double expected) {
            constexpr double tolerance = 1e-8;

            const bool passed =
                std::isfinite(actual) &&
                std::abs(actual - expected) <= tolerance;

            std::cout << (passed ? "PASS: " : "FAIL: ")
                << name << '\n';

            if (!passed) {
                std::cerr << "  Expected: " << expected
                    << ", actual: " << actual << '\n';
                ++failures;
            }
        };

    auto checkOutcome = [&](std::string_view name,
        const afrm::FlightInput& input,
        const afrm::FuelPolicy& settings,
        bool expectedSuccess) {
            afrm::FuelResults output{};

            const auto error =
                afrm::calculateFuel(input, settings, output);

            const bool succeeded = !error.has_value();
            const bool passed = succeeded == expectedSuccess;

            std::cout << (passed ? "PASS: " : "FAIL: ")
                << name << '\n';

            if (!passed) {
                if (error) {
                    std::cerr << "  Error: " << *error << '\n';
                }
                ++failures;
            }

            return passed;
        };

    // Test 1: Approved reference flight
    afrm::FuelResults result{};

    if (const auto error =
        afrm::calculateFuel(baseline, policy, result)) {
        std::cerr << "FAIL: Reference flight: "
            << *error << '\n';
        return 1;
    }

    check("Flight duration", result.durationHours, 1.0);
    check("Trip fuel", result.tripFuelKg, 200.0);
    check("Contingency fuel", result.contingencyFuelKg, 20.0);
    check("Total required fuel", result.totalRequiredFuelKg, 440.0);
    check("Destination balance", result.destinationBalanceKg, 260.0);
    check("Fuel margin", result.fuelMarginKg, 60.0);

    check("Planning range",
        result.theoreticalRangeNm,
        (280.0 / 220.0) * 200.0);

    check("Margin consistency",
        result.fuelMarginKg,
        result.destinationBalanceKg - policy.reserveFuelKg);

    // Test 2: Zero-distance boundary
    auto flight = baseline;
    flight.distanceNm = 0.0;

    if (checkOutcome("Zero-distance calculation",
        flight, policy, true)) {
        afrm::calculateFuel(flight, policy, result);

        check("Zero-distance trip fuel", result.tripFuelKg, 0.0);
        check("Zero-distance total required",
            result.totalRequiredFuelKg, 220.0);
    }

    // Test 3: Valid flight with insufficient fuel
    flight = baseline;
    flight.availableFuelKg = 400.0;

    if (checkOutcome("Insufficient-fuel calculation",
        flight, policy, true)) {
        afrm::calculateFuel(flight, policy, result);

        check("Negative fuel margin", result.fuelMarginKg, -40.0);
        check("Insufficient-fuel range",
            result.theoreticalRangeNm,
            (180.0 / 220.0) * 200.0);
    }

    // Test 4: No reserve-protected range available
    flight = baseline;
    flight.availableFuelKg = 200.0;

    if (checkOutcome("Reserve-limited calculation",
        flight, policy, true)) {
        afrm::calculateFuel(flight, policy, result);

        check("Zero theoretical range",
            result.theoreticalRangeNm, 0.0);
    }

    // Test 5: Invalid flight must be rejected
    flight = baseline;
    flight.groundSpeedKnots = 0.0;

    checkOutcome("Reject zero groundspeed",
        flight, policy, false);

    // Test 6: Invalid policy must be rejected
    auto invalidPolicy = policy;
    invalidPolicy.reserveFuelKg = -1.0;

    checkOutcome("Reject negative reserve",
        baseline, invalidPolicy, false);

    // Test 7: Detect arithmetic overflow
    flight = baseline;
    flight.distanceNm = std::numeric_limits<double>::max();
    flight.groundSpeedKnots = 1.0;

    checkOutcome("Reject calculation overflow",
        flight, policy, false);

    std::cout << "\nCalculation test failures: "
        << failures << '\n';

    return failures == 0 ? 0 : 1;
}
