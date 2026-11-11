
# AFRM Assignment 2: End-to-end threshold verification

if(NOT DEFINED AFRM_EXE OR NOT DEFINED DATA_FILE OR NOT DEFINED POLICY_MODE)
    message(FATAL_ERROR "Missing test configuration")
endif()

if(NOT POLICY_MODE STREQUAL "fixed" AND
   NOT POLICY_MODE STREQUAL "dynamic")
    message(FATAL_ERROR "Invalid policy mode")
endif()

# Execute the application and capture its results.
execute_process(
    COMMAND "${AFRM_EXE}" "${DATA_FILE}"
    RESULT_VARIABLE exit_code
    OUTPUT_VARIABLE output
    ERROR_VARIABLE errors
)

if(NOT "${exit_code}" STREQUAL "0")
    message(FATAL_ERROR
        "AFRM execution failed (${exit_code}): ${errors}")
endif()

# Normalise Windows line endings.
string(REPLACE "\r\n" "\n" output "${output}")

# Separate individual flight-output sections.
string(REPLACE "Flight: " ";Flight: " sections "${output}")

# Each case contains:
# ID | required fuel | margin | dynamic threshold |
# expected fixed status | expected dynamic status
set(cases
    "A2-REF|440.00|60.00|50.00|SUFFICIENT|SUFFICIENT"
    "A2-DYN-LOW|550.00|54.00|55.00|SUFFICIENT|CAUTION"
    "A2-DYN-AT|550.00|55.00|55.00|SUFFICIENT|SUFFICIENT"
    "A2-DYN-BELOW|550.00|54.99|55.00|SUFFICIENT|CAUTION"
    "A2-CROSS|501.00|50.00|50.10|SUFFICIENT|CAUTION"
    "A2-INSUF|550.00|-1.00|55.00|INSUFFICIENT|INSUFFICIENT"
    "A2-LARGE|1100.00|80.00|110.00|SUFFICIENT|CAUTION"
)

set(checked 0)

foreach(case IN LISTS cases)
    string(REPLACE "|" ";" fields "${case}")

    list(GET fields 0 id)
    list(GET fields 1 required)
    list(GET fields 2 margin)
    list(GET fields 3 threshold)

    if(POLICY_MODE STREQUAL "fixed")
        list(GET fields 4 expected_status)
    else()
        list(GET fields 5 expected_status)
    endif()

    set(found FALSE)

    foreach(section IN LISTS sections)
        string(FIND "${section}" "Flight: ${id}\n" start)

        if(start EQUAL 0)
            set(found TRUE)

            foreach(expected IN ITEMS
                "  Required fuel: ${required} kg\n"
                "  Fuel margin: ${margin} kg\n"
                "  Status: ${expected_status}\n"
            )
                string(FIND "${section}" "${expected}" position)

                if(position EQUAL -1)
                    message(FATAL_ERROR
                        "${id}: Missing or incorrect output: ${expected}")
                endif()
            endforeach()

            # The revised version must also display its threshold.
            if(POLICY_MODE STREQUAL "dynamic")
                string(FIND "${section}"
                    "  Caution threshold: ${threshold} kg\n"
                    position)

                if(position EQUAL -1)
                    message(FATAL_ERROR
                        "${id}: Incorrect dynamic caution threshold")
                endif()
            endif()

            math(EXPR checked "${checked} + 1")
            break()
        endif()
    endforeach()

    if(NOT found)
        message(FATAL_ERROR "Missing flight output: ${id}")
    endif()
endforeach()

# Check that no flight records are missing or duplicated.
string(REGEX MATCHALL "Flight: [^\n]+\n" headers "${output}")
list(LENGTH headers number_of_flights)

if(NOT number_of_flights EQUAL 7 OR
   NOT output MATCHES "Processed 7 flights")
    message(FATAL_ERROR "Expected exactly seven flight results")
endif()

message(STATUS "Validated ${checked} flights in ${POLICY_MODE} mode")