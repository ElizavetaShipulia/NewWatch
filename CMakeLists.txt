cmake_minimum_required(VERSION 3.20)

project(NetWatch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(NetWatch
    src/main.cpp
    src/core/logger.cpp
    src/core/config_manager.cpp
    src/network/network_scanner.cpp
)

target_include_directories(NetWatch PRIVATE
    include
    third_party
)
enable_testing()

find_package(GTest REQUIRED)

add_executable(NetWatchTests
    tests/network_scanner_test.cpp
    src/network/network_scanner.cpp
)

target_include_directories(NetWatchTests PRIVATE
    include
)

target_link_libraries(NetWatchTests PRIVATE
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(NetWatchTests)