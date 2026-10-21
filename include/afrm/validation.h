
#pragma once

#include "afrm/flight_data.h"

#include <optional>
#include <string>

namespace afrm {

    std::optional<std::string> validateFlightInput(
        const FlightInput& flight
    );

} // namespace afrm
