
cmake_minimum_required(VERSION 3.21)

project(AFRM LANGUAGES CXX)

# Main AFRM application
add_executable(afrm
    src/main.cpp
)

# Automated test executable
add_executable(afrm_tests
    tests/test_setup.cpp
)

# Configure both targets to use C++20
set_target_properties(afrm afrm_tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# MSVC compiler diagnostics
if(MSVC)
    target_compile_options(afrm PRIVATE
        /W4
        /permissive-
    )

    target_compile_options(afrm_tests PRIVATE
        /W4
        /permissive-
    )
endif()

# Enable automated testing
enable_testing()

# Register the initial test
add_test(
    NAME afrm_test_infrastructure
    COMMAND afrm_tests
)
