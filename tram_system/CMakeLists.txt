cmake_minimum_required(VERSION 3.10)
project(tram_system)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(tram_system
    src/commands.cpp
    src/main.cpp
    src/tram.cpp
    src/stop.cpp
    src/tram_system.cpp
)