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

# --- 2. Define the Core Library (The Engine) ---
include_directories(include)
add_library(monkdb_lib src/hashmap.cpp)

# --- 3. The Test Suite ---
enable_testing()
add_executable(monkdb_test src/hashmap_test.cpp)
target_link_libraries(monkdb_test PRIVATE monkdb_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(monkdb_test)

# --- 4. The Benchmark (The Race Car) ---
# This creates a NEW executable target that CLion can see and run.
# IMPORTANT: Make sure your file is named 'src/benchmark_hashmap.cpp'.
# If you named it 'src/benchmark_hashmap.cpp', change the filename below.
add_executable(monkdb_benchmark src/benchmark_hashmap.cpp)
target_link_libraries(monkdb_benchmark PRIVATE monkdb_lib)