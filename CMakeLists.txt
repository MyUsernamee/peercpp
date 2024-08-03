cmake_minimum_required(VERSION 3.8)
project(peercpp)

include (FetchContent)

set(CMAKE_CXX_STANDARD 17)

find_package(LibDataChannel REQUIRED)

# Nlohmann JSON
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY
  https://github.com/nlohmann/json.git
        GIT_TAG
        v3.11.3
)

FetchContent_MakeAvailable(nlohmann_json)

# Soruces
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Library target
add_library(peercpp
        ${SOURCES}
        )

target_include_directories(peercpp PUBLIC src)
target_include_directories(peercpp PUBLIC ${nlohmann_json_SOURCE_DIR}/include)

target_link_libraries(peercpp
        LibDataChannel::LibDataChannel
        )

# Examples
add_subdirectory(examples)