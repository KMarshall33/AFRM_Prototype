
cmake_minimum_required(VERSION 3.21)

project(AFRM LANGUAGES CXX)

# Shared AFRM application logic
add_library(afrm_core STATIC
    src/validation.cpp
 "include/afrm/fuel_calculator.h")

# Make header files accessible
target_include_directories(afrm_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Main AFRM executable
add_executable(afrm
    src/main.cpp
 "include/afrm/fuel_calculator.h")

target_link_libraries(afrm PRIVATE afrm_core)

# Existing testing infrastructure
add_executable(afrm_tests
    tests/test_setup.cpp
 "include/afrm/fuel_calculator.h")

# Configure all targets consistently
foreach(target afrm_core afrm afrm_tests)
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

# Enable automated testing
enable_testing()

add_test(
    NAME afrm_test_infrastructure
    COMMAND afrm_tests
)

# AFRM input-validation tests
add_executable(afrm_validation_tests
    tests/test_validation.cpp
 "include/afrm/fuel_calculator.h")

target_link_libraries(afrm_validation_tests PRIVATE afrm_core)

set_target_properties(afrm_validation_tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(MSVC)
    target_compile_options(afrm_validation_tests PRIVATE
        /W4
        /permissive-
    )
endif()

add_test(
    NAME afrm_input_validation
    COMMAND afrm_validation_tests
)
