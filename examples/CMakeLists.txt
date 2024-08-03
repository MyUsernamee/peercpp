
cmake_minimum_required(VERSION 3.8)
project(peercppexamples)

include (FetchContent)

set(CMAKE_CXX_STANDARD 17)

# Soruces
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Nlohmann JSON
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY
  https://github.com/nlohmann/json.git
        GIT_TAG
        v3.11.3
)

FetchContent_MakeAvailable(nlohmann_json)

# Library target
add_executable(main_test src/main.cpp)
add_executable(test_datachannel src/test_datachannel.cpp)

target_include_directories(main_test PUBLIC src)
target_include_directories(test_datachannel PUBLIC src)
target_include_directories(test_datachannel PUBLIC ${nlohmann_json_SOURCE_DIR}/include)

target_link_libraries(main_test
        peercpp
        )

        
# Examples
