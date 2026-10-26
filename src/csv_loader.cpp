
#include "afrm/csv_loader.h"
#include "afrm/validation.h"

#include <array>
#include <charconv>
#include <cmath>
#include <cctype>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

namespace afrm {
    namespace {

        // Remove whitespace from the beginning and end of a field.
        std::string_view trim(std::string_view text)
        {
            while (!text.empty() &&
                std::isspace(static_cast<unsigned char>(text.front()))) {
                text.remove_prefix(1);
            }

            while (!text.empty() &&
                std::isspace(static_cast<unsigned char>(text.back()))) {
                text.remove_suffix(1);
            }

            return text;
        }

        // Split one CSV line into comma-separated fields.
        std::vector<std::string_view> splitFields(std::string_view line)
        {
            std::vector<std::string_view> fields;

            std::size_t start = 0;

            while (true) {
                const auto comma = line.find(',', start);

                if (comma == std::string_view::npos) {
                    fields.push_back(trim(line.substr(start)));
                    break;
                }

                fields.push_back(trim(line.substr(start, comma - start)));
                start = comma + 1;
            }

            return fields;
        }

        // Convert a complete text field into a finite double.
        bool parseNumber(std::string_view text, double& value)
        {
            if (text.empty()) {
                return false;
            }

            const char* begin = text.data();
            const char* end = begin + text.size();

            const auto result = std::from_chars(
                begin, end, value, std::chars_format::general
            );

            return result.ec == std::errc{} &&
                result.ptr == end &&
                std::isfinite(value);
        }

    } // anonymous namespace

    CsvLoadResult loadFlightsCsv(std::istream& input)
    {
        CsvLoadResult result{};

        constexpr std::array<std::string_view, 6> expectedHeader{
            "flight_id",
            "distance_nm",
            "groundspeed_knots",
            "fuel_burn_kg_per_hour",
            "available_fuel_kg",
            "taxi_fuel_kg"
        };

        constexpr std::array<std::string_view, 5> numericNames{
            "distance_nm",
            "groundspeed_knots",
            "fuel_burn_kg_per_hour",
            "available_fuel_kg",
            "taxi_fuel_kg"
        };

        std::string line;
        std::size_t lineNumber = 1;

        // The first line must contain the expected header.
        if (!std::getline(input, line)) {
            result.errors.push_back({ 1, "Missing CSV header" });
            return result;
        }

        // Accept an optional UTF-8 byte-order mark.
        if (line.compare(0, 3, "\xEF\xBB\xBF") == 0) {
            line.erase(0, 3);
        }

        const auto headerFields = splitFields(line);

        bool headerValid =
            headerFields.size() == expectedHeader.size();

        if (headerValid) {
            for (std::size_t i = 0; i < expectedHeader.size(); ++i) {
                if (headerFields[i] != expectedHeader[i]) {
                    headerValid = false;
                    break;
                }
            }
        }

        if (!headerValid) {
            result.errors.push_back({
                1, "CSV header does not match the required columns"
                });
            return result;
        }

        // Process each data record.
        while (std::getline(input, line)) {
            ++lineNumber;

            // Ignore completely blank or whitespace-only lines.
            if (trim(line).empty()) {
                continue;
            }

            // Quoted CSV fields are outside this parser's scope.
            if (line.find('"') != std::string::npos) {
                result.errors.push_back({
                    lineNumber, "Quoted CSV fields are not supported"
                    });
                continue;
            }

            const auto fields = splitFields(line);

            if (fields.size() != expectedHeader.size()) {
                result.errors.push_back({
                    lineNumber, "Expected exactly 6 CSV fields"
                    });
                continue;
            }

            FlightInput flight{};
            flight.flightId = std::string(fields[0]);

            const std::array<double*, 5> numericValues{
                &flight.distanceNm,
                &flight.groundSpeedKnots,
                &flight.fuelBurnKgPerHour,
                &flight.availableFuelKg,
                &flight.taxiFuelKg
            };

            bool numbersValid = true;

            for (std::size_t i = 0; i < numericValues.size(); ++i) {
                if (!parseNumber(fields[i + 1], *numericValues[i])) {
                    result.errors.push_back({
                        lineNumber,
                        "Invalid number in column " +
                            std::string(numericNames[i])
                        });

                    numbersValid = false;
                    break;
                }
            }

            if (!numbersValid) {
                continue;
            }

            // Reuse the validation implemented in Phase 4.1.
            if (const auto error = validateFlightInput(flight)) {
                result.errors.push_back({ lineNumber, *error });
                continue;
            }

            result.flights.push_back(std::move(flight));
        }

        if (result.flights.empty() && result.errors.empty()) {
            result.errors.push_back({
                1, "CSV file contains no flight records"
                });
        }

        return result;
    }

} // namespace afrm
