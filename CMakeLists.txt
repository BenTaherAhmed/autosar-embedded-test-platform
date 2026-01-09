cmake_minimum_required(VERSION 3.20)
project(autosar_test_platform C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

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
