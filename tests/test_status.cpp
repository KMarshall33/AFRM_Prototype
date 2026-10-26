
#include "afrm/fuel_calculator.h"
#include "afrm/fuel_status.h"

#include <cmath>
#include <iostream>
#include <limits>
#include <string_view>

int main()
{
    int failures = 0;

    using afrm::FuelStatus;

    // Test an individual classification
    auto check = [&](std::string_view name,
        double margin,
        double threshold,
        FuelStatus expected) {
            const auto actual =
                afrm::classifyFuelStatus(margin, threshold);

            const bool passed =
                actual.has_value() && *actual == expected;

            std::cout << (passed ? "PASS: " : "FAIL: ")
                << name << '\n';

            if (!passed) {
                std::cerr << "  Expected: "
                    << afrm::fuelStatusToString(expected)
                    << '\n';
                ++failures;
            }
        };

    // Test rejection of invalid classification inputs
    auto checkInvalid = [&](std::string_view name,
        double margin,
        double threshold) {
            const auto actual =
                afrm::classifyFuelStatus(margin, threshold);

            const bool passed = !actual.has_value();

            std::cout << (passed ? "PASS: " : "FAIL: ")
                << name << '\n';

            if (!passed) {
                ++failures;
            }
        };

    constexpr double caution = 50.0;

    // Direct classification and boundary tests
    check("Negative margin", -40.0, caution,
        FuelStatus::Insufficient);

    check("Immediately below zero", -0.01, caution,
        FuelStatus::Insufficient);

    check("Exactly zero margin", 0.0, caution,
        FuelStatus::Caution);

    check("Small positive margin", 0.01, caution,
        FuelStatus::Caution);

    check("Below caution threshold", 49.99, caution,
        FuelStatus::Caution);

    check("Exactly 50 kg", 50.0, caution,
        FuelStatus::Sufficient);

    check("Above caution threshold", 60.0, caution,
        FuelStatus::Sufficient);

    // Closest representable double below 50
    const double justBelow50 =
        std::nextafter(50.0, 0.0);

    check("Next representable value below 50",
        justBelow50, caution,
        FuelStatus::Caution);

    // Alternative valid threshold
    check("Zero threshold and zero margin",
        0.0, 0.0,
        FuelStatus::Sufficient);

    // Invalid classification parameters
    checkInvalid("Negative caution threshold",
        60.0, -1.0);

    checkInvalid("NaN fuel margin",
        std::numeric_limits<double>::quiet_NaN(),
        caution);

    checkInvalid("Infinite fuel margin",
        std::numeric_limits<double>::infinity(),
        caution);

    checkInvalid("Infinite caution threshold",
        60.0,
        std::numeric_limits<double>::infinity());

    // Check classification using the actual calculator
    const afrm::FlightInput baseline{
        "AFRM-001", 200.0, 200.0, 200.0, 500.0, 20.0
    };

    const afrm::FuelPolicy policy{};

    auto checkFlight = [&](std::string_view name,
        double availableFuel,
        double expectedMargin,
        FuelStatus expectedStatus) {
            auto flight = baseline;
            flight.availableFuelKg = availableFuel;

            afrm::FuelResults results{};

            const auto error =
                afrm::calculateFuel(flight, policy, results);

            if (error) {
                std::cerr << "FAIL: " << name
                    << " - " << *error << '\n';
                ++failures;
                return;
            }

            const auto status = afrm::classifyFuelStatus(
                results.fuelMarginKg,
                policy.cautionThresholdKg
            );

            const bool marginCorrect =
                std::isfinite(results.fuelMarginKg) &&
                std::abs(results.fuelMarginKg - expectedMargin)
                <= 1e-8;

            const bool statusCorrect =
                status.has_value() &&
                *status == expectedStatus;

            const bool passed =
                marginCorrect && statusCorrect;

            std::cout << (passed ? "PASS: " : "FAIL: ")
                << name << '\n';

            if (!passed) {
                std::cerr << "  Expected margin: "
                    << expectedMargin
                    << ", actual: "
                    << results.fuelMarginKg << '\n';
                ++failures;
            }
        };

    checkFlight("Reference flight",
        500.0, 60.0, FuelStatus::Sufficient);

    checkFlight("Exact sufficient boundary",
        490.0, 50.0, FuelStatus::Sufficient);

    checkFlight("Below sufficient boundary",
        489.0, 49.0, FuelStatus::Caution);

    checkFlight("Exact reserve boundary",
        440.0, 0.0, FuelStatus::Caution);

    checkFlight("Insufficient by 1 kg",
        439.0, -1.0, FuelStatus::Insufficient);

    checkFlight("Insufficient by 40 kg",
        400.0, -40.0, FuelStatus::Insufficient);

    // Verify conversion from enum to output text
    if (afrm::fuelStatusToString(FuelStatus::Insufficient)
        != "INSUFFICIENT" ||
        afrm::fuelStatusToString(FuelStatus::Caution)
        != "CAUTION" ||
        afrm::fuelStatusToString(FuelStatus::Sufficient)
        != "SUFFICIENT") {
        std::cerr << "FAIL: Status text conversion\n";
        ++failures;
    }
    else {
        std::cout << "PASS: Status text conversion\n";
    }

    std::cout << "\nStatus test failures: "
        << failures << '\n';

    return failures == 0 ? 0 : 1;
}
