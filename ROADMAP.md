# MonkDB Roadmap

**Philosophy:** We do not build a Ferrari on Day 1. We build a Skateboard, then a Bike, then a Car, then a Ferrari. This roadmap is organized by **Maturity Level**, prioritizing correctness before optimization.
## 🐣 Phase 1: The "Fragile" In-Memory Engine (Make it Work)

**The Goal:** A functional Key-Value store that lives entirely in RAM. It works perfectly as long as you don't turn off the computer.

**The Focus:** Logic correctness, Pointer Arithmetic, Linked Data Structures.

## Epic 1: Core Data Structures
### Epic 1.1: The Warmup (HashMap) [DONE]
  - [X] **Design Doc:** Open Addressing, Linear Probing, Tombstones   
  - [X] **Skeleton:** CMake setup, Header definition (`hashmap.h`).
  - [X] **Implementation:** `FindSlot` (Linear Probe engine).
  - [X] **Implementation:** `Put` / `Get` / `Delete` with Load Factor check.
  - [X] **Implementation:** `Resize` (Stop-the-world).
  - [X] **Test:** Verify correctness with GoogleTest.
  - [X] **Benchmark:** vs std::unordered_map.

### Epic 1.2: The Dumb Arena (Memory)
*Goal: Stop using `malloc` for every node. Establish the memory layout.* 
  - [ ] **Story 1.2.1:** Build `SimpleArena`.
    - [X] Design Doc: [design_arena.md]
      - Critical Question: How do we enforce 8-byte alignment? 
      - Critical Question: What is the policy for >4MB allocations?
    - [ ] Implementation: SimpleArena class.
      - [ ] Allocate 4MB blocks using `new char[]`.
      - [ ] Implement `Allocate(bytes)` that simply bumps a pointer, Alignment Math ((ptr + 7) & ~7)..
    - [ ] Test: arena_test.cc
      - [ ] Verify Allocate(1) followed by Allocate(8) returns aligned address.
    - [ ] **Anti-Goal:** Do not implement "Huge Pages" or "Alignment" yet. Just get the math right.

### Epic 1.3: The Single-Threaded Skip List (Index)
*Goal: Sorted data structure for Range Scans.*
- [ ] **Story 1.3.1:** Implement Core Skip List Logic.
  - [ ] Design Doc: [design_skiplist.md]
    - Critical Question: Node Layout (Flexible Array Member). 
    - Critical Question: rand() vs Bitwise height generation trade-offs.
  - [ ] Use `rand()` for height generation (Simple).
  - [ ] Node struct with char* keys (Arena offsets).
  - [ ] Use `std::mutex` for thread safety (Global Lock).
  - [ ] Implement `Put` (Insert) and `Get` (Search).
  - [ ] Test: skiplist_test.cc
    - [ ] Correctness: Insert 10k items, verify Contains returns true.
    - [ ] Ordering: Iterate and verify keys are sorted "A" -> "Z".
  - [ ] **Anti-Goal:** No CAS. No Lock-Free complexity.

### Epic 1.4: The Iterator
*Goal: Enable scanning.*
- [ ] **Story 1.4.1:** Implement `Seek`, `Next`, and `Valid`.
  - [ ] Verify you can print keys "A" through "Z" in order.

> **🎓 Learning Outcome:** Deep understanding of Pointer Arithmetic and Linked Data Structures.
> 
> **✅ The Test:** Insert 1 million keys. Read them back. Check memory usage (should be stable).
---

## 💾 Phase 2: The "Durable" Engine (Make it Crash - Safe)
**The Goal:** A database that survives a crash. If I pull the power plug, I lose nothing.
**The Focus:** Sequential Disk I/O, Serialization, Crash Recovery.

### Epic 2.1: The Write Ahead Log (WAL)
- [ ] **Story 2.1.1:** Define Binary Log Format.
  - [ ] Design Doc: [design_format.md]
    - Critical Question: Varint Encoding (how to compress int). 
    - Critical Question: Record Format (CRC|Type|Key|Val).
  - [ ] **Story 2.1.1:** Data Serialization.
    - [ ] Implement `EncodeVarint32` / `DecodeVarint32`.
    - [ ] Define Record Format: `[CRC | Type | KeyLen | Key | ValLen | Value]`.
- [ ] **Story 2.1.2:** WAL Writer.
  [ ] **Test:** `coding_test.cc` (Varint round-trip).
- [ ] **Anti-Goal:** Do NOT buffer writes in RAM (LogBuffer). Write directly to file.
 
- [ ] **Story 2.1.2:** Implement WAL Writer.
  - [ ] Design Doc: [design_wal.md]
    - Critical Question: Handling partial writes/corruption?
  - [ ] Append bytes to `wal.log` *before* modifying MemTable.
  - [ ] Call `fsync()` to force physical write.
  - [ ] Test: wal_test.cc (The "Kill-9" Test).
    - [ ] Write data. Crash process. Restart. Verify data exists. File size check
### Epic 2.2: Crash Recovery
- [ ] **Story 2.2.1:** Implement WAL Reader.
  - [ ] On startup, read `wal.log`.
  - [ ] Verify CRCs. Stop at first corruption (EOF).
  - [ ] Replay valid insertions into the SkipList to restore state.
    [ ] **Test:** The "Kill-9" Test.
    - [ ] Write 1000 keys. `kill -9` process. Restart. Verify keys exist.
> **🎓 Learning Outcome:** Sequential I/O vs Random I/O, Serialization, Durability guarantees.

---

## 📚 Phase 3: The "Scalable" Engine (Make it Big)
**The Goal:** Store more data than RAM allows. The transition from "Cache" to "Database."
**The Focus:** LSM Tree architecture, External Sorting, File Management.

### Epic 3.1: The Flush (RAM -> Disk)
- [ ] **Story 3.1.1:** Implement Flush Trigger.
  - [ ] Design Doc: [design_sstable.md]
    - Critical Question: SSTable Format (Data Blocks + Index Block).
  - [ ] When MemTable hits 64MB, freeze it (Immutable).
  - [ ] Iterate and write to a sorted file (`00001.sst`).
  - [ ] Clear MemTable and delete old WAL.
  - [ ] Test: sstable_test.cc
    - [ ] Flush "a".."z". Verify file size.

### Epic 3.2: The Read Path (Disk Lookup)
- [ ] **Story 3.2.1:** Multi-Layered Get.
  - [ ] Check MemTable -> Check Immutable MemTable -> Check SSTable files.
  - [ ] Implement Binary Search over the SSTable file.

### Epic 3.3: Basic Compaction[CUT]
- [ ] Story 3.3.1: The Manifest
  - Simple text file tracking live SSTables.
  - Without a "Manifest" (or file registry), when you merge file_1.sst and file_2.sst into file_3.sst, readers might still be trying to read file_1.
  - [ ] **Story 3.3.2:** Stop-the-World Merge.
    - [ ] **Design Doc:** `[design_compaction.md]`
      - *Critical Question:* K-Way Merge Sort logic.
    - [ ] Trigger: When `N` SSTables exist (e.g., 10), pause writes.
    - [ ] Merge all 10 files -> 1 New Large File.
    - [ ] Delete old files. Resume writes.
    - [ ] **Anti-Goal:** Do NOT build a background thread pool or concurrent compaction. Pausing the DB for 1 second is acceptable for Cycle 1.
    - [ ] **Test:** `compaction_test.cc`
      - [ ] Create 5 overlapping small files. Call `Compact()`. Verify 1 sorted file remains.
- [ ] **Story 3.3.3:** Implement Merge.[CUT]
  - [ ] Design Doc: [design_compaction.md]
    - Critical Question: K-Way Merge Sort logic.
  - [ ] Implementation: TableReader & CompactionJob.
    - [ ] Implement mmap reader (Basic).
    - [ ] Implement BinarySearch over SSTable index.
    - [ ] Background Job: Merge 2 SSTables -> 1 New SSTable (Drop Tombstones). If L0 has too many files, merge them into one larger L1 file. Handle Tombstones (physically remove deleted keys during merge).
  - [ ] Test: compaction_test.cc
    - [ ] Create 2 overlapping files. Merge. Verify 1 sorted file remains.


> **🎓 Learning Outcome:** Read/Write Amplification, External Merge Sort, Immutable Files.
> **✅ The Test (The "Overflow"):** Set RAM limit to 64MB. Insert 1GB of data. Verify no OOM crash and data exists in `.sst` files.

---

## 🚀 Phase 4: The "High-Performance" Engine (Make it Fast)
**The Goal:** Maximize Operations Per Second (OPS). Remove bottlenecks.
**The Focus:** CPU Caches, Concurrency, Hardware Optimizations.

### Epic 4.1: Smart Arena (Hardware Aware) [CUT]
- [ ] **Story 4.1.1:** Reduce Overhead.
  - [ ] Design Doc: [design_hardware.md] Update.
    - Critical Question: Huge Pages benefits?
- [ ] Use `mmap` with `MAP_HUGETLB` (Huge Pages) to reduce TLB thrashing instead of `new char[]`.

### Epic 4.2: Lock-Free Skip List [CUT]
- [ ] **Story 4.2.1:** Implement Atomics.
  - [ ] Design Doc: [design_concurrency.md]
    - Critical Question: Memory Ordering (Release/Acquire). 
    - Critical Question: Harris Protocol (Bit Marking).
  - [ ] Replace `std::mutex` with `std::atomic` and CAS (Compare-And-Swap).
  - [ ] Replace rand() with std::mt19937 + ctz (Bitwise height).
  - [ ] Test: concurrency_test.cc
    - [ ] Thread Sanitizer run: 10 threads inserting simultaneously.

### Epic 4.3: Read Optimizations[CUT]
- [ ] **Story 4.3.1:** Bloom Filters.
  - [ ] Design Doc: [design_cache.md]
  - [ ] Build Bloom Filter for every SSTable to avoid expensive disk reads for non-existent keys.
- [ ] **Story 4.3.2:** Block Cache.
  - [ ] Implement Thread Safe LRU Cache for frequently accessed SSTable blocks.
- [ ] Benchmark: Compare Phase 3 vs Phase 4 (Expect 10x-50x gain).

> **🎓 Learning Outcome:** Memory Models, Atomics, CPU Cache Coherence, Probabilistic Data Structures.
> **✅ The Test (The "Benchmark"):** Compare Phase 3 vs Phase 4 throughput using `perf`. Look for 10x-50x improvement.

---

## 🛡️ Phase 5: The "Production" Engine (Make it Maintainable)
**The Goal:** A system you can trust in production.
**The Focus:** Safety, Observability, Edge Cases.

### Epic 5.1: The Server & Maintenance
  - [ ] Design Doc: [design_server.md]
  - [ ] Implementation: monkdb_server.
  - [ ] Main loop.
  - [ ] Background Thread for Compaction (The "Cron Job").

### Epic 5.2: Data Integrity [CUT]
- [ ] **Story 5.2.1:** Checksums.
  - [ ] Implement CRC32C for every node/block.
  - [ ] Panic/Crash immediately if data corruption is detected.

### Epic 5.3: Flow Control[CUT]
- [ ] **Story 5.3.1:** Write Stalls.
  - [ ] Sleep (1ms) if the disk cannot keep up with MemTable flushing, (prevent OOM).
  - [ ] Prevent OOM crashes under heavy load.

### Epic 5.4: Observability [CUT]
- [ ] **Story 5.4.1:** Metrics & Logs.
  - [ ] Expose P99/P95 Latency, Write Throughput, Cache Hit Rate.
  - [ ] Structured JSON Logging.
  - [ ] **Story 5.4.2: The Final Exam**
    - [ ] Load Generator (Write/Read mix).
    - [ ] Compare Linear Probing vs. Baseline.
    - [ ] **Optimization Decision:** Switch to Quadratic Probing/Incremental Resize *only* if P99 latency is unacceptable.
    - [ ] Final Benchmark Report.

> **🎓 Learning Outcome:** Reliability Engineering, Backpressure, Debugging Black Box systems.
> **✅ The Test (The "Chaos"):** Run load generator for 24h. Randomly pause disk/corrupt RAM. Ensure safe recovery or failure.

---

## 🏆 Summary of the Journey

| Phase | Name | Key Concept | The "Vibe" |
| :--- | :--- | :--- | :--- |
| **1** | **In-Memory** | Pointers & Logic | *"It works on my machine!"* |
| **2** | **Durability** | Sequential I/O | *"I can restart it!"* |
| **3** | **Storage** | LSM Tree & Files | *"I can store 100GB!"* |
| **4** | **Speed** | Atomics & Caching | *"It handles 100k req/sec!"* |
| **5** | **Production** | Safety & Metrics | *"I trust it with customer data."* |