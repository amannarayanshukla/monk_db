//
// Created by aman shukla on 01/01/26.
//

#pragma once
#include <string>
#include <optional>

namespace monkdb {

    // Generic SkipList or specific for KV?
    // For this stage, let's keep it specific to string/string to avoid template hell
    // while you learn the structure. Refactor to templates later.
    class SkipList {
    public:
        SkipList();
        ~SkipList();

        // Core API
        void Put(const std::string& key, const std::string& value);
        std::optional<std::string> Get(const std::string& key);
        bool Delete(const std::string& key);

    private:
        // TODO: Define Node structure
        // TODO: Define random height generator
        struct Node;
        Node* head_;
    };

}