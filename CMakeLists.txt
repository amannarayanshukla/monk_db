cmake_minimum_required(VERSION 3.14)
project(monkdb)

set(CMAKE_CXX_STANDARD 17)

# --- 1. Fetch GoogleTest automatically ---
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# Force shared CRT to avoid windows/linux linking issues
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- 2. Define the Core Library ---
# We compile hashmap.cpp into a library called "monkdb_lib"
include_directories(include)
add_library(monkdb_lib src/hashmap.cpp
        src/benchmark_hashmap.cpp
)

# --- 3. The Test Suite ---
enable_testing()

# Create the test executable
add_executable(monkdb_test src/hashmap_test.cpp)

# Link the test runner to your library and GoogleTest
target_link_libraries(monkdb_test PRIVATE monkdb_lib gtest_main)

# Register tests so CLion sees them
include(GoogleTest)
gtest_discover_tests(monkdb_test)