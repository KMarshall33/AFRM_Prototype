
#pragma once

#include "afrm/flight_data.h"

#include <optional>
#include <string>

namespace afrm {

    struct FuelResults {
        double durationHours{};
        double tripFuelKg{};
        double contingencyFuelKg{};
        double totalRequiredFuelKg{};
        double destinationBalanceKg{};
        double fuelMarginKg{};
        double theoreticalRangeNm{};
    };

    std::optional<std::string> calculateFuel(
        const FlightInput& flight,
        const FuelPolicy& policy,
        FuelResults& results
    );

} // namespace afrm
