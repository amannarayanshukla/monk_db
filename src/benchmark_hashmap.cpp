//
// Created by aman shukla on 08/01/26.
//

#include <monkdb/hashmap.h>
#include <chrono>
#include <iostream>
#include <string>
#include <random>
#include <unordered_map>
#include <vector>

// Using 1 Million items to get statistically significant data
const int NUM_ITEMS = 1000000;

void BenchmarkHashmap() {
    std::cout << "--- MonkDB Benchmark (Items: " << NUM_ITEMS << ") ---" << std::endl;

    std::cout << "Generating random keys..." << std::endl;
    std::vector<std::string> keys;
    for (int i = 0; i < NUM_ITEMS; i++) {
        keys.push_back("key" + std::to_string(i));
    }

    // === Contender 1: MonkDB HashMap ===
    {
        std::cout << "\n[MonkDB::HashMap]" << std::endl;
        monkdb::HashMap myMap(16);// Start small to force resizing

        // Write Benchmark
        auto start = std::chrono::high_resolution_clock::now();
        for (const auto& key : keys) {
            myMap.Put(key, "static_value");
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "  Write: " << duration << " ms" << std::endl;

        // Read Benchmark
        start = std::chrono::high_resolution_clock::now();
        volatile int found = 0; // 'volatile' prevents compiler from optimizing this loop away
        for (const auto& key : keys) {
            if (myMap.Get(key).has_value()) {
                found++;
            }
        }

        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "  Read: " << duration << " ms" << std::endl;
    }

    // === Reference: std::unordered_map ===
    std::cout << "\n[std::unordered_map]" << std::endl;
    std::unordered_map<std::string, std::string> referenceMap;

    // Write Benchmark
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& key : keys) {
        referenceMap[key] = "static_value";
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "  Write: " << duration << " ms" << std::endl;

    // READ TEST
    start = std::chrono::high_resolution_clock::now();
    volatile int found = 0;
    for (const auto& key : keys) {
        if (referenceMap.find(key) != referenceMap.end()) {
            found++;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "  Read:  " << duration << " ms" << std::endl;
}

int main() {
    BenchmarkHashmap();
    return 0;
}
