
#pragma once

#include <string>

namespace afrm {

    struct FlightInput {
        std::string flightId;
        double distanceNm{};
        double groundSpeedKnots{};
        double fuelBurnKgPerHour{};
        double availableFuelKg{};
        double taxiFuelKg{};
    };

    struct FuelPolicy {
        double contingencyFraction{ 0.10 };
        double reserveFuelKg{ 200.0 };
        double cautionThresholdKg{ 50.0 };
    };

} // namespace afrm
