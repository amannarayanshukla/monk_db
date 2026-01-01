add_executable(monkdb_tests
        skiplist_test.cpp
)

target_link_libraries(monkdb_tests
        PRIVATE
        monkdb_core
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(monkdb_tests)