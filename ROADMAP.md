# MonkDB Roadmap

## Phase 1: In-Memory Engine (Volatile)
### Epic 1: Core Data Structures
- [ ] **Story 1.1: The Warmup (HashMap)**
    - [ ] Setup CMake and GTest infrastructure.
    - [ ] Implement basic Put/Get wrapper.
    - [ ] Benchmark vs std::unordered_map.
- [ ] **Story 1.2: The MemTable (Skip List)**
    - [ ] Define Node structure (flexible height).
    - [ ] Implement `Put` (Insertion with random height).
    - [ ] Implement `Get` (Search).
    - [ ] Implement `Delete`.
    - [ ] Implement Iterator (Start, Next, Valid, Key, Value).

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
### Epic 5: Metrics & Benchmarking
- [ ] **Story 5.1: Structured Logging**
    - [ ] Implement JSON Logger.
- [ ] **Story 5.2: Latency Tracking**
    - [ ] Implement P95/P99 Histogram.
- [ ] **Story 5.3: The Final Exam**
    - [ ] Load Generator (Write/Read mix).
    - [ ] Final Benchmark Report.