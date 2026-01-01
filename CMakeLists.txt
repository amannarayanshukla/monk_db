cmake_minimum_required(VERSION 3.0)
project(MonkDB VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. GoogleTest Setup (Downloads it automatically)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

# 2. The Core Library (libmonkdb)
add_library(monkdb
        src/skiplist.cpp
)
target_include_directories(monkdb PUBLIC include)

# 3. The Executable (Server)
add_executable(monkdb_server src/main.cpp)
target_link_libraries(monkdb_server PRIVATE monkdb_core)

# 4. Tests
enable_testing()
add_subdirectory(tests)