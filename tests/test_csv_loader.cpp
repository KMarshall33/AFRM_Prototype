
#include "afrm/csv_loader.h"

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

int main()
{
    int failures = 0;

    const std::string header =
        "flight_id,distance_nm,groundspeed_knots,"
        "fuel_burn_kg_per_hour,available_fuel_kg,taxi_fuel_kg\n";

    auto check = [&](std::string_view name,
        const std::string& csv,
        std::size_t expectedFlights,
        std::size_t expectedErrors,
        std::size_t expectedFirstErrorLine = 0) {
            std::istringstream input(csv);
            const auto result = afrm::loadFlightsCsv(input);

            bool passed =
                result.flights.size() == expectedFlights &&
                result.errors.size() == expectedErrors;

            if (expectedFirstErrorLine != 0 &&
                !result.errors.empty()) {
                passed = passed &&
                    result.errors.front().lineNumber ==
                    expectedFirstErrorLine;
            }

            std::cout << (passed ? "PASS: " : "FAIL: ")
                << name << '\n';

            if (!passed) {
                std::cerr << "  Flights: " << result.flights.size()
                    << ", errors: " << result.errors.size()
                    << '\n';
                ++failures;
            }
        };

    check(
        "Two valid records",
        header +
        "AFRM-001,200,200,200,500,20\n"
        "AFRM-002,200,200,200,490,20\n",
        2, 0
    );

    check(
        "Whitespace around fields",
        header + " AFRM-001 , 200 , 200 , 200 , 500 , 20 \n",
        1, 0
    );

    check(
        "Blank lines ignored",
        header + "\nAFRM-001,200,200,200,500,20\n\n",
        1, 0
    );

    check(
        "Missing header",
        "",
        0, 1, 1
    );

    check(
        "Incorrect header",
        "id,distance,speed\n",
        0, 1, 1
    );

    check(
        "No flight records",
        header,
        0, 1, 1
    );

    check(
        "Missing CSV field",
        header + "AFRM-001,200,200,200,500\n",
        0, 1, 2
    );

    check(
        "Extra CSV field",
        header + "AFRM-001,200,200,200,500,20,10\n",
        0, 1, 2
    );

    check(
        "Non-numeric groundspeed",
        header + "AFRM-001,200,invalid,200,500,20\n",
        0, 1, 2
    );

    check(
        "Zero groundspeed rejected",
        header + "AFRM-001,200,0,200,500,20\n",
        0, 1, 2
    );

    check(
        "Non-finite fuel input",
        header + "AFRM-001,200,200,nan,500,20\n",
        0, 1, 2
    );

    check(
        "Out-of-range number",
        header + "AFRM-001,200,200,1e9999,500,20\n",
        0, 1, 2
    );

    check(
        "Quoted fields rejected",
        header + "\"AFRM-001\",200,200,200,500,20\n",
        0, 1, 2
    );

    check(
        "Multiple invalid records",
        header +
        "AFRM-001,200,invalid,200,500,20\n"
        "AFRM-002,200,200,200,-1,20\n",
        0, 2, 2
    );

    check(
        "Valid record followed by invalid record",
        header +
        "AFRM-001,200,200,200,500,20\n"
        "AFRM-002,200,invalid,200,490,20\n",
        1, 1, 3
    );

    std::cout << "\nCSV test failures: "
        << failures << '\n';

    return failures == 0 ? 0 : 1;
}
