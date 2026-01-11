# MonkDB Roadmap

## Phase 1: In-Memory Engine (Volatile)
### Epic 1: Core Data Structures
- [ ] **Story 1.1: The Warmup (HashMap)**
    - [X] **Design Doc:** Open Addressing, Linear Probing, Tombstones   
    - [X] **Skeleton:** CMake setup, Header definition (`hashmap.h`).
    - [X] **Implementation:** `FindSlot` (Linear Probe engine).
    - [X] **Implementation:** `Put` / `Get` / `Delete` with Load Factor check.
    - [X] **Implementation:** `Resize` (Stop-the-world).
    - [X] **Test:** Verify correctness with GoogleTest.
    - [X] **Benchmark:** vs std::unordered_map.
- [ ] **Story 1.2: The MemTable (Skip List)**
    - [ ] **Story 1.2: The MemTable (Skip List)**
    - [ ] **Design Doc:** Architecture decisions.
      - *Key Decisions:* Arena Memory Layout, "Flexible Array Member" for Nodes, Append-Only (Tombstone) Deletion.
    - [ ] **Skeleton:** Header definitions (`skiplist.h`, `arena.h`) and CMake updates.
    - [ ] **Implementation: The Arena**
      - [ ] Build `SimpleArena` (Vector of 4KB blocks).
      - [ ] Implement `Allocate(bytes)` (Bump pointer).
    - [ ] **Implementation: The Node**
      - [ ] Define `Node` struct with Flexible Array Member (`Node* next[1]`).
      - [ ] Use `char*` (Arena offsets) instead of `std::string`.
    - [ ] **Implementation: Core Engine**
      - [ ] `Put`: Insert with random height (Append-Only).
      - [ ] `Get`: Search with predecessor logic.
      - [ ] **Constraint:** No `Delete` function (use Tombstones).
    - [ ] **Implementation: Iterator**
      - [ ] Forward Iterator (`SeekToFirst`, `Next`, `Valid`).
    - [ ] **Test:** Verify correctness (Ordering, Range Scans) with GoogleTest.
    - [ ] **Benchmark:** Compare Insertion/Scan speed vs `std::map`.
---

## Phase 2: Durability (Crash-Safe)
### Epic 2: Write Ahead Log (WAL)
- [ ] **Story 2.1: Data Serialization**
    - [ ] Implement `EncodeVarint32`.
    - [ ] Implement `DecodeVarint32`.
- [ ] **Story 2.2: The Append-Only Log**
    - [ ] Define Log Record Format (CRC|Len|Type|Key|Val).
    - [ ] Implement `WALWriter::Append` with `fsync`.
- [ ] **Story 2.3: Crash Recovery**
    - [ ] Implement `WALReader` (iterate over log file).
    - [ ] Replay valid logs into SkipList on startup.

---

## Phase 3: Storage & Maintenance (Persistent)
### Epic 3: Storage Engine
- [ ] **Story 3.1: Memory Management**
    - [ ] Implement Arena/Slab allocator (optional optimization).
- [ ] **Story 3.2: SSTables (Sorted String Tables)**
    - [ ] Flush MemTable to disk (SSTable format).
    - [ ] Implement External Merge Sort (Compaction).
- [ ] **Story 3.3: Fast I/O**
    - [ ] Implement `mmap` reader for SSTables.

---

## Phase 4: Read Optimization (Fast)
### Epic 4: Caching & Filtering
- [ ] **Story 4.1: Block Cache**
    - [ ] Implement Thread-Safe LRU Cache.
    - [ ] Integrate Cache into Get() path.
- [ ] **Story 4.2: Bloom Filter**
    - [ ] Implement MurmurHash.
    - [ ] Build Bloom Filter on SSTable flush.
    - [ ] Check Bloom Filter before disk read.

---

## Phase 5: Observability & Production
### Epic 5: Server, Metrics & Benchmarking
- [ ] **Story 5.1: The Server Executable**
  - [ ] Create `monkdb_server` (main.cpp).
  - [ ] **Optimization:** Background thread for Compaction/Cleanup (The "Cron Job" for Hashmap, which will help create indexes).
- [ ] **Story 5.1: Structured Logging**
    - [ ] Implement JSON Logger.
- [ ] **Story 5.2: Latency Tracking**
    - [ ] Implement P95/P99 Histogram.
- [ ] **Story 5.3: The Final Exam**
    - [ ] Load Generator (Write/Read mix).
    - [ ] Compare Linear Probing vs. Baseline.
    - [ ] **Optimization Decision:** Switch to Quadratic Probing/Incremental Resize *only* if P99 latency is unacceptable.
    - [ ] Final Benchmark Report.

## Out of Scope (Future Work)
- Consistent Hashing(required for clustering, part of distributed architecture)