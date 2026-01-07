#include "include/monkdb/hashmap.h"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "MonkDB Starting..." << std::endl;

	monkdb::HashMap monkMap(10);
	// Test Put and Get, happy path
	monkMap.Put("key1", "value1");
	std::optional<std::string> result = monkMap.Get("key1");
	assert(result.has_value()); // Die here if empty
	assert(result == "value1"); // Die here if wrong value

	// Testing Update existing key
	monkMap.Put("key1", "value2");
	result = monkMap.Get("key1");
	assert(result.has_value()); // Die here if empty
	assert(result == "value2"); // Die here if wrong value

	// Delete key
	monkMap.Delete("key1");
    result = monkMap.Get("key1");
	assert(!result.has_value()); // Should be empty after delete

	// Resize test
	monkdb::HashMap smallMap(100);
	for(int i = 0; i < 200; i++) {
        smallMap.Put("key" + std::to_string(i), "value" + std::to_string(i));
    }

	assert(smallMap.Size() == 200);
	assert(smallMap.Capacity() >= 200);
	assert(smallMap.Get("key0").has_value());
	assert(smallMap.Get("key150").has_value());

	std::cout << "All tests passed!" << std::endl;
    return 0;
}