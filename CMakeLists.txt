cmake_minimum_required(VERSION 3.20)
project(autosar_test_platform C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

cmake_minimum_required(VERSION 3.20)
project(autosar_test_platform C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ===== Coverage option (GCC/Clang only) =====
option(ENABLE_COVERAGE "Enable code coverage (GCC/Clang)" OFF)

if(ENABLE_COVERAGE AND MSVC)
    message(FATAL_ERROR "ENABLE_COVERAGE is not supported with MSVC. Use GCC/Clang (WSL/MinGW).")
endif()

if(ENABLE_COVERAGE)
    message(STATUS "Coverage enabled (GCC/Clang)")
endif()


# Library (mock MCAL)
add_library(mcal_mock
    src/mcal_mock/spi.c
    src/mcal_mock/can.c
)

target_include_directories(mcal_mock PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mcal_mock
)

# Unit test executable - SPI
add_executable(test_spi
    tests/unit_c/test_spi.c
)
target_link_libraries(test_spi PRIVATE mcal_mock)

# Unit test executable - CAN
add_executable(test_can
    tests/unit_c/test_can.c
)
target_link_libraries(test_can PRIVATE mcal_mock)

enable_testing()
add_test(NAME test_spi COMMAND test_spi)
add_test(NAME test_can COMMAND test_can)

# ===== Apply coverage flags to targets (only if enabled) =====
if(ENABLE_COVERAGE)
    target_compile_options(mcal_mock PRIVATE --coverage -O0 -g)
    target_link_options(mcal_mock PRIVATE --coverage)

    target_compile_options(test_spi PRIVATE --coverage -O0 -g)
    target_link_options(test_spi PRIVATE --coverage)

    target_compile_options(test_can PRIVATE --coverage -O0 -g)
    target_link_options(test_can PRIVATE --coverage)
endif()

