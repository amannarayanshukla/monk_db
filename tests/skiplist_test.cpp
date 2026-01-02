//
// Created by aman shukla on 01/01/26.
//
#include <gtest/gtest.h>
#include "monkdb/skiplist.h"

TEST(SkipListTest, BasicPutGet) {
    monkdb::SkipList sl;
    sl.Put("foo", "bar");

    // This will fail until you implement Get()
    // auto val = sl.Get("foo");
    // ASSERT_TRUE(val.has_value());
    // EXPECT_EQ(*val, "bar");
}