# Design Document: The MemTable (LSM Staging Buffer)

**Author:** Aman Shukla
**Date:** 2026-01-21
**Status:** DRAFT

## 1. Core Architecture

### 1.1 Memory Management Strategy (The Arena)
* **Decision:** [MALLOC vs. ARENA?]
* **Block Size:** [SIZE?]
    * *Constraint Check:* Why did you choose this size? Why not 4KB (Page Size)? Why not 64MB continuous?
* **Alignment Strategy:** [HOW?]
    * *Constraint Check:* How will you ensure pointers returned by `Allocate` start on 8-byte boundaries to avoid CPU split-locks?

### 1.2 Data Structure Selection
* **Structure:** [NAME?]
* **Why not B-Tree?**
    * *Constraint Check:* B-Trees have better Cache Locality. Why are we rejecting them in favor of a pointer-heavy structure? Address Write Concurrency.
* **Why not Red-Black Tree?**
    * *Constraint Check:* Why is the standard `std::map` (RB-Tree) bad for high-throughput concurrent writes? Address Rotation Locking.

### 1.3 Node Memory Layout (The "Fat Pointer")
* **Key Storage:** [std::string vs. Raw Bytes?]
* **Pointer Storage:** [std::vector vs. Flexible Array Member?]
* **Reasoning:**
    * *Constraint Check:* How does your design avoid calling `malloc` implicitly inside the Node constructor?

---

## 2. Algorithms & Logic

### 2.1 Insertion Logic
* **Duplicate Handling:** [Overwrite In-Place vs. Append New Node?]
* **Height Generation:** [Method?]
    * *Constraint Check:* `rand() % 4` is banned due to modulo overhead and global state. What is the bitwise alternative?

### 2.2 Deletion Logic
* **Mechanism:** [Physical Remove vs. Tombstone?]
* **Justification:**
    * *Constraint Check:* Why can't we physically reclaim memory holes in an Arena-based system?

### 2.3 Concurrency Strategy
* **Phase 1 (MVP):** [Locking Strategy?]
* **Phase 2 (Target):** [Lock-Free Strategy?]
* **ABA Problem:**
    * *Constraint Check:* Explain why the specific properties of the Arena design make the ABA problem mathematically impossible here.

---

## 3. Implementation Specs (Skeleton)

### 3.1 The Arena Class
*Draft the class definition. Focus on the `Allocate` signature and state management.*

```cpp
class Arena {
public:
    // [Fill in: Allocation Function Signature]
    // Note: Must return aligned memory.

    // [Fill in: Memory Usage Metric]
private:
    // [Fill in: Data structure to hold the list of blocks]

    // [Fill in: Pointers to track