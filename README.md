# MonkDB (Phase 1)

**MonkDB** is an embedded key-value store built in C++17, currently implementing a high-performance in-memory HashMap with Linear Probing.

## 📁 Project Structure

* `include/monkdb/` - Header files (`hashmap.h`).
* `src/` - Implementation files (`hashmap.cpp`) and tests.
* `CMakeLists.txt` - Build configuration for GoogleTest.

---

## 🏗️ Method 1: The Manual Build (Quick Check)
Use this method to run your custom `hashmap_smoke_testing_main.cpp` script (the "Old Test").

**Command:**
```bash
g++ src/hashmap_smoke_testing_main.cpp src/hashmap.cpp -o test_runner -I. -std=c++17
```
**Run:**
```bash
./test_runner
```

-------
### ⚠️ Important Note on Headers
When compiling manually with -I. (Include Current Directory), you must include headers using the full path from the root:

C++

// Correct for Manual Build:
#include "include/monkdb/hashmap.h"
------
## 🧪 Method 2: The GoogleTest Suite (Recommended)
Use this method to run the automated regression tests. This uses CMake to handle dependencies and linking.
**Build Steps:**
```bash
mkdir build
cd build
cmake ..
make
```
**Run Tests:**
```bash
./monkdb_test
```
This will execute all GoogleTest cases defined in `hashmap_test.cpp` and report results.

----
### ⚠️ Important Note on Headers
When compiling with CMake (include_directories(include)), the compiler looks inside the include folder automatically. You must update your imports:

C++

// Correct for CMake/GoogleTest:
#include "monkdb/hashmap.h"

------
## 🛠️ Usage (Manual)
To create your own runner, ensure you use the correct namespace:
```cpp
#include "include/monkdb/hashmap.h" // Manual build path
#include <iostream>

int main() {
    monkdb::HashMap db(10);
    db.Put("key1", "value1");
    
    auto result = db.Get("key1");
    if(result) {
        std::cout << "Found: " << *result << std::endl;
    }
    return 0;
}
```

## ⚡ Performance Benchmarks

### HASHMAP LINEAR PROBING vs UNORDERED_MAP (CHAINING)
We benchmarked MonkDB against the C++ Standard Library (`std::unordered_map`) using **1,000,000 keys** with string payloads.
* **Environment:** Release Build (`-O3`), MacOS/Linux
* **Dataset:** 1M unique string keys, pre-generated.

| Operation | [HashMap]MonkDB (Linear Probing) | std::unordered_map (Chaining) | Result |
| :--- |:---------------------------------| :--- | :--- |
| **Insert (Write)** | **150 ms**                       | 194 ms | 🏆 **~22% Faster** |
| **Lookup (Read)** | 49 ms                            | **40 ms** | ~18% Slower |

**Why the difference?**
* **Writes:** MonkDB[HashMap] uses a contiguous memory block (`std::vector`). It avoids the overhead of 1,000,000 separate heap allocations (`malloc`) that `std::unordered_map` requires for its linked-list nodes.
* **Reads:** `std::unordered_map` wins slightly on reads because chaining avoids the "scan" cost of Linear Probing during collisions, though MonkDB remains highly competitive due to CPU cache locality.