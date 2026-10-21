
cmake_minimum_required(VERSION 3.21)

project(AFRM LANGUAGES CXX)

# Build the AFRM executable
add_executable(afrm
    src/main.cpp
)

# Use C++20
set_target_properties(afrm PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Configure MSVC compiler diagnostics
if(MSVC)
    target_compile_options(afrm PRIVATE
        /W4
        /permissive-
    )
endif()
