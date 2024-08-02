cmake_minimum_required(VERSION 3.8)
project(peercpp)

set(CMAKE_CXX_STANDARD 17)

find_package(LibDataChannel REQUIRED)

# Soruces
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Library target
add_library(peercpp
        ${SOURCES}
        )

target_include_directories(peercpp PUBLIC src)

target_link_libraries(peercpp
        LibDataChannel::LibDataChannel
        )

# Examples
add_subdirectory(examples)