# Design Document: High-Performance In-Memory Hash Map

**Author:** Aman Shukla
**Date:** 2026-01-05
**Status:** IN PROGRESS

## 1. Core Architecture

### 1.1 Data Structure Selection
**Decision:** Open Addressing
**Reasoning:**
Since Cache locality is a priority for performance, open addressing is chosen over separate chaining. 
This approach minimizes pointer chasing and leverages contiguous memory allocation, which is more CPU cache-friendly.
For issues like clustering, we will implement quadratic probing to reduce primary clustering effects.
and we will use tombstones to handle deletions effectively. But we know that tombstones can lead to performance degradation over time,
so we will implement a rehashing strategy when the load factor exceeds a certain threshold, to avoid zombie apocalypse.
### 1.2 Hash Function
**Decision:** MurmurHash3
**Reasoning:**
MurmurHash3 is selected for its excellent balance between speed and low collision rates. 
It is non-cryptographic, making it suitable for hash maps where security is not a primary concern.
### 1.3 Seed Management
**Decision:** Siphash-based Seed Generation
**Reasoning:**
To mitigate HASH DoS attacks, we will use a Siphash-based approach to generate a unique seed for the hash function at the initialization of the hash map.
Also since currently we are not supporting distributed systems, we can keep it simple by generating a random seed at startup.
Once we move to distributed systems, we can revisit this decision to ensure consistency across nodes.
Note that this decision causes and issue when distributed systems are considered, as different nodes may generate different seeds, leading to inconsistent hashing.
---

## 2. Collision Resolution

### 2.1 Algorithm
**Decision:** Quadratic Probing
**Reasoning:**
Quadratic probing is chosen to resolve collisions as it helps to reduce primary clustering, which can occur with linear probing.
Probing is approx equal to 1/1-load_factor, so we need to keep load factor in check.
We can keep track of the probe count during insertions and lookups to monitor performance.
If max probe count exceeds log2 (capacity), we can trigger a rehash to maintain performance.
### 2.2 The Delete Problem (Tombstones)
**Mechanism:**
We will use tombstones to mark deleted entries. This allows us to maintain the integrity of the probing sequence.
However, to prevent performance degradation due to excessive tombstones i.e. if data + tombstones > 70% of capacity, we will trigger a rehash to clean up the table.
During rehashing, we will skip over tombstones and only copy active entries to the new table.
## 3. Memory Management

### 3.1 Load Factor & Resizing
**Threshold:** 0.7
**Growth Factor:** 2.0x
**Reasoning:**
We will monitor the load factor of the hash map and trigger a resize operation when it exceeds 0.7.
Resizing involves creating a new larger array and rehashing all existing entries into this new array
to maintain performance. The new capacity will be 2.0 times the current capacity to balance memory usage and performance.
The reason for choosing 0.7 as the load factor threshold is to strike a balance between memory efficiency and performance.
Higher load factors can lead to increased collisions and longer probe sequences, which degrade performance.
And the reason for choosing 2.0x growth factor is to reduce the frequency of resizing operations while still keeping memory usage reasonable.
Since resizing is an expensive operation, we want to minimize how often it occurs.
### 3.2 Rehash Strategy
**Mechanism:**
There are two scenarios that will trigger a rehash:
1. Load factor exceeds 0.7 during insertion [LATER], currently we will do stop-the-world rehashing.
2. Excessive tombstones leading to performance degradation.
During rehashing, we will create a new array with double the capacity and re-insert all active entries from the old array into the new one.
This process will skip over tombstones to ensure that the new table is clean and efficient.

Rehash Strategy we will be implemented as follows:
1. Allocate new array with double capacity.
2. Do incremental rehashing: on each insertion or lookup, move a small number of entries from the old table to the new table.
3. Once all entries are moved, switch to the new table and deallocate the old one [LATER], currently we will do stop-the-world rehashing.
4. We will also implement a background CRON job that would do the same incremental rehashing during periods of no look up and insertion activity. [LATER]
6. The steps to do during the rehashing are:
   - Allocate new array with double capacity.
   - When a request is made (insertion or lookup), move a small number of entries (e.g., 10) from the old table to the new table.
   - Then resolve the current request (insertion or lookup) in the new table.
   - For each entry in the old array:
     - If the entry is active (not a tombstone), re-insert it into the new array using the hash function and probing strategy.
   - Once all entries are moved, switch to the new table and deallocate the old one.
7. The trade off of this approach is that it spreads out the cost of rehashing over multiple operations, hence the TAX is paid gradually rather than all at once. So each request may take slightly longer during the rehashing phase, but the overall system remains responsive. And for the period of the rehashing, both the old and new tables will consume memory, which is a trade-off for maintaining performance.
## 4. Interface Definition (API)

```cpp
class HashMap {
public:
    Get(const std::string& key) -> std::optional<std::string>;
    Put(const std::string& key, const std::string& value) -> void;
    Delete(const std::string& key) -> bool;
    // What private members?
private:
    Enum class EntryState { ACTIVE, DELETED, EMPTY };
    struct Entry {
        std::string key;
        std::string value;
        EntryState state;
    };
    std::vector<Entry> table_;
    size_t capacity_;
    size_t size_;
};
```