
#pragma once

#include "afrm/flight_data.h"

#include <cstddef>
#include <istream>
#include <string>
#include <vector>

namespace afrm {

    struct CsvError {
        std::size_t lineNumber{};
        std::string message;
    };

    struct CsvLoadResult {
        std::vector<FlightInput> flights;
        std::vector<CsvError> errors;
    };

    CsvLoadResult loadFlightsCsv(std::istream& input);

} // namespace afrm
