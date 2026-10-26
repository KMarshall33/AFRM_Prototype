
cmake_minimum_required(VERSION 3.21)

project(AFRM LANGUAGES CXX)

# Shared AFRM application logic
add_library(afrm_core STATIC
    src/validation.cpp
    src/fuel_calculator.cpp
    src/fuel_status.cpp
    src/csv_loader.cpp
)

target_include_directories(afrm_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Main application
add_executable(afrm
    src/main.cpp
)

target_link_libraries(afrm PRIVATE afrm_core)

# Infrastructure test
add_executable(afrm_tests
    tests/test_setup.cpp
)

# Input validation tests
add_executable(afrm_validation_tests
    tests/test_validation.cpp
)

target_link_libraries(afrm_validation_tests
    PRIVATE afrm_core
)

# Fuel calculation tests
add_executable(afrm_calculation_tests
    tests/test_calculations.cpp
)

target_link_libraries(afrm_calculation_tests
    PRIVATE afrm_core
)

# Fuel status tests
add_executable(afrm_status_tests
    tests/test_status.cpp
)

target_link_libraries(afrm_status_tests
    PRIVATE afrm_core
)

# CSV parser tests
add_executable(afrm_csv_tests
    tests/test_csv_loader.cpp
)

target_link_libraries(afrm_csv_tests
    PRIVATE afrm_core
)

# C++20 and compiler diagnostics
foreach(target
    afrm_core
    afrm
    afrm_tests
    afrm_validation_tests
    afrm_calculation_tests
    afrm_status_tests
    afrm_csv_tests
)
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )

    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4
            /permissive-
        )
    endif()
endforeach()

# Automated testing
enable_testing()

add_test(
    NAME afrm_test_infrastructure
    COMMAND afrm_tests
)

add_test(
    NAME afrm_input_validation
    COMMAND afrm_validation_tests
)

add_test(
    NAME afrm_fuel_calculations
    COMMAND afrm_calculation_tests
)

add_test(
    NAME afrm_fuel_status
    COMMAND afrm_status_tests
)

add_test(
    NAME afrm_csv_parser
    COMMAND afrm_csv_tests
)

# End-to-end test using the actual application and dataset
add_test(
    NAME afrm_csv_example
    COMMAND afrm
        "${CMAKE_CURRENT_SOURCE_DIR}/data/flights.csv"
)

set_tests_properties(afrm_csv_example PROPERTIES
    PASS_REGULAR_EXPRESSION "Processed 6 flights"
)
