
cmake_minimum_required(VERSION 3.8)
project(peercppexamples)

set(CMAKE_CXX_STANDARD 17)

# Soruces
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Library target
add_executable(peercppexamples
        ${SOURCES}
        )

target_include_directories(peercppexamples PUBLIC include)

target_link_libraries(peercppexamples
        peercpp
        )

        
# Examples
