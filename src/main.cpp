#include "monkdb/skiplist.h"
#include <iostream>

int main() {
    std::cout << "MonkDB Starting..." << std::endl;

    monkdb::SkipList memtable;
    memtable.Put("key1", "value1");

    return 0;
}