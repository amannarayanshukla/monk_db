# Design: The Dumb Arena (Memory Allocator)
**Status:** DRAFT
**Author:** Aman Shukla

## 1. The Problem
*Why are we building a custom allocator instead of just using `new`/`malloc` or `std::vector` for every node? What are the two specific performance killers we are trying to avoid?*
> Syscall Overhead: Every time you call new or malloc, the OS has to find free RAM. This takes time. If you insert 1 million keys, that is 1 million requests to the OS. 
> 
> * Arena Fix: We ask the OS for 4MB once. Then we carve out small pieces ourselves (just bumping a pointer). It turns 1 million syscalls into 1 syscall. 
> 
> Cache Locality (Fragmentation): If you new Node() 1 million times, the nodes are scattered randomly all over your RAM. Your CPU has to jump around to find them. 
> 
> * Arena Fix: Nodes are packed  tightly next to each other in that 4MB block. Reading them is sequential and fast.

## 2. Core Decisions

### 2.1 Block Allocation Strategy
* **Block Size:** [Decide: 4KB, 4MB, or 1GB? Justify your choice based on syscall overhead vs. memory hoarding.]
> **Block Size:** 4MB
> * *Justification:* *
> ** **Vs 4KB:** Avoids excessive `malloc` syscalls and pointer metadata overhead for millions of small allocations.
> ** **Vs 64MB:** Avoids "Memory Hoarding" where a mostly-empty arena holds onto a massive chunk of RAM, preventing other components from using it. 4MB amortizes the syscall cost while keeping memory usage granular.
> * * **Alignment Strategy:** How will you ensure pointers returned by `Allocate` start on 8-byte boundaries to avoid CPU split-locks?
> * * *Constraint Check:* We enforce 8-byte alignment by rounding up the allocation pointer. We effectively add padding bytes (waste) to the previous allocation to ensure the next pointer starts at an address divisible by 8 (e.g., addr & 7 == 0). This prevents unaligned memory access and split-lock penalties


* **Data Structure:** How will you store these blocks? 
> We will use std::vector<char*> and not LL because we want to store the pointer to each block together and when we move from one block to the next for filling it we don't want to lose the pointer to the earlier block, as we will only move the current_ptr and the starting pointer for the earlier block will stay in that vector and can be easily read/flushed   

* **Logic:** When `Allocate(bytes)` is called and the current block is full, what is the exact sequence of steps?
> 1. Mark current block is filled
> 2. Request new block from the OS
> 3. Add the pointer to the std::vector<char*>
> 3. Move the current_ptr to this new block

### 2.2 Alignment Strategy (The Math)
* **Constraint:** We need 8-byte alignment for atomic operations later.
* **The Math:** If `current_ptr` is at address `1001` and the user asks for 8 bytes, what address do you return? What is the bitwise formula to calculate this generically?
> 1001 % 8 is 1 but will return would add padding from 1001 to 1007 and then return 1008
> result = (ptr + 7) & ~7; (i.e first we add to overshoot and then &~7 brings the last 3 bits to 0, which is the case for any number multiple of 8)

### 2.3 Edge Case: Large Allocations
* **Scenario:** User calls `Allocate(10MB)` but your block size is `4MB`.
* **Policy:** [Do you reject it? Do you allocate a special "Oversized" block? What is the simplest solution for Cycle 1?]
> Will abort for now, if request > 4MB, will handle in next cycle, with addition of a new flow where we request the block from OS for the given size

## 3. Interface (API)
*Draft the C++ class definition.*


```cpp
class Arena {
public:
    // 1. Give me memory (Append Only)
    char* Allocate(size_t bytes);
    // 2. How much RAM are we hoarding? (For the Flush Trigger)
    size_t MemoryUsage() const;
};
```

### NOTES:
    Arena is Type blind it just knows bytes
    Arena is Time Ordered not Sorted
    Arena doesn't support Free or Delete
    