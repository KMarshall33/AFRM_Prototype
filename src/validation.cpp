
#include "afrm/validation.h"

#include <cmath>

namespace afrm {

    std::optional<std::string> validateFlightInput(
        const FlightInput& flight)
    {
        if (flight.flightId.empty()) {
            return "Flight identifier must not be empty";
        }

        if (!std::isfinite(flight.distanceNm) ||
            flight.distanceNm < 0.0) {
            return "Distance must be finite and non-negative";
        }

        if (!std::isfinite(flight.groundSpeedKnots) ||
            flight.groundSpeedKnots <= 0.0) {
            return "Groundspeed must be finite and greater than zero";
        }

        if (!std::isfinite(flight.fuelBurnKgPerHour) ||
            flight.fuelBurnKgPerHour <= 0.0) {
            return "Fuel burn rate must be finite and greater than zero";
        }

        if (!std::isfinite(flight.availableFuelKg) ||
            flight.availableFuelKg < 0.0) {
            return "Available fuel must be finite and non-negative";
        }

        if (!std::isfinite(flight.taxiFuelKg) ||
            flight.taxiFuelKg < 0.0) {
            return "Taxi fuel must be finite and non-negative";
        }

        if (flight.taxiFuelKg > flight.availableFuelKg) {
            return "Taxi fuel cannot exceed available fuel";
        }

        return std::nullopt;
    }

} // namespace afrm
