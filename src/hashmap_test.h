#include <gtest/gtest.h>
#include "monkdb/hashmap.h"

// Test Case 1: Basic Operations
TEST(HashMapTest, PutAndGet) {
    monkdb::HashMap map(10);
    map.Put("key1", "val1");
    
    auto result = map.Get("key1");
    ASSERT_TRUE(result.has_value());      // Fatal failure if empty
    EXPECT_EQ(*result, "val1");           // Non-fatal if wrong value
    
    auto missing = map.Get("ghost");
    EXPECT_FALSE(missing.has_value());
}

// Test Case 2: Updates
TEST(HashMapTest, UpdateKey) {
    monkdb::HashMap map(10);
    map.Put("key1", "val1");
    map.Put("key1", "val2"); // Overwrite
    
    auto result = map.Get("key1");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "val2");
}

// Test Case 3: Delete
TEST(HashMapTest, DeleteKey) {
    monkdb::HashMap map(10);
    map.Put("key1", "val1");
    
    bool deleted = map.Delete("key1");
    EXPECT_TRUE(deleted);
    
    auto result = map.Get("key1");
    EXPECT_FALSE(result.has_value());
    
    // Deleting again should fail
    EXPECT_FALSE(map.Delete("key1"));
}

// Test Case 4: Resize Logic
TEST(HashMapTest, ResizeStressTest) {
    monkdb::HashMap map(10); // Start small
    int limit = 100;
    
    // Insert 100 items to force resize
    for (int i = 0; i < limit; i++) {
        map.Put("key" + std::to_string(i), "val" + std::to_string(i));
    }
    
    // Verify Size
    EXPECT_EQ(map.Size(), limit);
    EXPECT_GT(map.Capacity(), 10); // Capacity should have grown
    
    // Verify random access still works (checking boundary conditions)
    EXPECT_EQ(*map.Get("key0"), "val0");
    EXPECT_EQ(*map.Get("key50"), "val50");
    EXPECT_EQ(*map.Get("key99"), "val99");
}