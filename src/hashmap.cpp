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
        // House keeping: Resize if load factor exceeds 0.7
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

    bool HashMap::Delete(const std::string &key) {
        // NOTE: No house keeping done when we are calling DELETE, because Load Factor doesnt change on deletes
        size_t index = FindSlot(key);

        if (table_[index].state == OCCUPIED && table_[index].key == key) {
            table_[index].state = DELETED; // Don't decrease size_ for simplicity, because of Ghost Problem(i.e. tombstones)
            return true;
        }
        return false; // Key not found
    }

    std::optional<std::string> HashMap::Get(const std::string &key) {
        // NOTE: No house keeping done when we are calling GET, since READS are more frequent than WRITES
        size_t index = FindSlot(key);

        if (table_[index].state == OCCUPIED && table_[index].key == key) {
            return table_[index].value; // Key found
        }
        return std::nullopt; // Key not found
    }


}