cmake_minimum_required(VERSION 3.10)
project(BMPConsolePrinter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main application
add_executable(bmp_printer
    src/bmp.cpp
    src/main.cpp
)

# BMP generator executable
add_executable(bmp_generator
    src/generator.cpp
)
target_include_directories(bmp_generator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Custom target for generation
add_custom_target(generate
    COMMAND bmp_generator test.bmp 32 32
    DEPENDS bmp_generator
    COMMENT "Generating test BMP file"
)

# Include directories
target_include_directories(bmp_printer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Windows specific settings
if (WIN32)
    target_compile_definitions(bmp_printer PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_definitions(bmp_generator PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()