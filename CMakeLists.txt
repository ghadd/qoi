cmake_minimum_required(VERSION 3.14)
project(QOIconv)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Check if the compiler is MSVC or GCC/Clang
if(MSVC)
    # MSVC uses /std:c++20
    add_compile_options("/std:c++20")
else()
    # GCC and Clang use -std=c++20
    add_compile_options("-std=c++20")
endif()

# Add source files for the program
add_executable(qoiconv qoiconv.c)
target_include_directories(qoiconv PRIVATE ${CMAKE_SOURCE_DIR} stb)

add_executable(cli cli.cpp)
target_include_directories(cli PRIVATE ${CMAKE_SOURCE_DIR} stb)

# Optionally, set properties like the output directory
set_target_properties(qoiconv cli PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
