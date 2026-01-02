//
// Created by aman shukla on 01/01/26.
//
#include "monkdb/skiplist.h"
#include <iostream>

namespace monkdb {

    SkipList::SkipList() {
        // TODO: Initialize head node
        std::cout << "[SkipList] Initialized" << std::endl;
    }

    SkipList::~SkipList() {
        // TODO: Cleanup nodes
    }

    void SkipList::Put(const std::string& key, const std::string& value) {
        // TODO: Implement insertion logic
        std::cout << "[SkipList] Put: " << key << " = " << value << std::endl;
    }

    std::optional<std::string> SkipList::Get(const std::string& key) {
        // TODO: Implement search logic
        return std::nullopt;
    }

    bool SkipList::Delete(const std::string& key) {
        // TODO: Implement delete logic
        return false;
    }

}