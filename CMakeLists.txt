cmake_minimum_required(VERSION 3.20)

project(NetWatch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(NetWatch
    src/main.cpp
    src/core/logger.cpp
    src/core/config_manager.cpp
)

target_include_directories(NetWatch PRIVATE
    include
    third_party
)