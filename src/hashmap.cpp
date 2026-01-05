//
// Created by aman shukla on 02/01/26.
//

#include "monkdb/hashmap.h"
#include <functional> // We will need this later for std::hash

namespace monkdb {
    HashMap::HashMap(size_t capacity){
        capacity_ = capacity;
        size_ = 0;
        table_.resize(capacity_);
    }

    std::size_t HashMap::FindSlot(const std::string &key) {
        // Simple hash function using std::hash
        std::hash<std::string> hasher;
        size_t hashValue = hasher(key);
        size_t index = hashValue % capacity_;

        // Linear probing
        while (table_[index].state != EMPTY) {
            // Check for EMPTY or matching key
            if (table_[index].state == OCCUPIED && table_[index].key == key) {
                return index; // Found the key
            }
            index = (index + 1) % capacity_;
        }
        return index; // Return the first available slot
    }

    void HashMap::Put(const std::string &key, const std::string &value) {
        float loadFactor = static_cast<float>(size_) / capacity_;
        if (loadFactor >= 0.7) {
            Resize(capacity_ * 2);
        }
        size_t index = FindSlot(key);

        // Update existing key
        if (table_[index].state == OCCUPIED && table_[index].key == key) {
            table_[index].value = value;
            return;
        }

        // Insert new key, for either EMPTY or DELETED slots
        if (table_[index].state != OCCUPIED) {
            size_++;
            table_[index].key = key;
            table_[index].value = value;
            table_[index].state = OCCUPIED;
            return;
        }
    }

}