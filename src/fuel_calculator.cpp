
#include "afrm/fuel_calculator.h"
#include "afrm/validation.h"

#include <cmath>

namespace afrm {

    std::optional<std::string> calculateFuel(
        const FlightInput& flight,
        const FuelPolicy& policy,
        FuelResults& results)
    {
        // 1. Validate the flight inputs
        if (const auto error = validateFlightInput(flight)) {
            return error;
        }

        // 2. Validate the simulated fuel policy
        if (!std::isfinite(policy.contingencyFraction) ||
            policy.contingencyFraction < 0.0) {
            return "Contingency fraction must be finite and non-negative";
        }

        if (!std::isfinite(policy.reserveFuelKg) ||
            policy.reserveFuelKg < 0.0) {
            return "Reserve fuel must be finite and non-negative";
        }

        if (!std::isfinite(policy.cautionThresholdKg) ||
            policy.cautionThresholdKg < 0.0) {
            return "Caution threshold must be finite and non-negative";
        }

        // 3. Calculate flight duration
        const double duration =
            flight.distanceNm / flight.groundSpeedKnots;

        // 4. Calculate trip fuel
        const double tripFuel =
            duration * flight.fuelBurnKgPerHour;

        // 5. Calculate contingency fuel
        const double contingencyFuel =
            tripFuel * policy.contingencyFraction;

        // 6. Calculate total required fuel
        const double requiredFuel =
            flight.taxiFuelKg +
            tripFuel +
            contingencyFuel +
            policy.reserveFuelKg;

        // 7. Calculate remaining fuel margin
        const double fuelMargin =
            flight.availableFuelKg - requiredFuel;

        // 8. Calculate conservative destination balance
        const double destinationBalance =
            flight.availableFuelKg -
            flight.taxiFuelKg -
            tripFuel -
            contingencyFuel;

        // 9. Calculate theoretical planning range
        const double fuelForRange =
            flight.availableFuelKg -
            flight.taxiFuelKg -
            policy.reserveFuelKg;

        const double adjustedBurn =
            flight.fuelBurnKgPerHour *
            (1.0 + policy.contingencyFraction);

        // Detect arithmetic overflow or underflow
        if (!std::isfinite(duration) ||
            !std::isfinite(tripFuel) ||
            !std::isfinite(contingencyFuel) ||
            !std::isfinite(requiredFuel) ||
            !std::isfinite(fuelMargin) ||
            !std::isfinite(destinationBalance) ||
            !std::isfinite(fuelForRange) ||
            !std::isfinite(adjustedBurn) ||
            adjustedBurn <= 0.0) {
            return "Fuel calculation exceeded supported numeric range";
        }

        const double theoreticalRange =
            fuelForRange <= 0.0
            ? 0.0
            : (fuelForRange / adjustedBurn) *
            flight.groundSpeedKnots;

        if (!std::isfinite(theoreticalRange)) {
            return "Range calculation exceeded supported numeric range";
        }

        // 10. Store results only after successful calculation
        results = FuelResults{
            duration,
            tripFuel,
            contingencyFuel,
            requiredFuel,
            destinationBalance,
            fuelMargin,
            theoreticalRange
        };

        return std::nullopt;
    }

} // namespace afrm
