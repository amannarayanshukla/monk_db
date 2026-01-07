**Build & Run Test**
```g++ src/main.cpp src/hashmap.cpp -o test_runner -I. -std=c++17```
and then execute:
```./test_runner```

The reason for listing two files here is that 
1. main.cpp contains the test cases
2. hashmap.cpp contains the implementation of the HashMap class.
Including both files ensures that when we compile, the test cases have access to the HashMap implementation to verify its correctness.
3. -I Look in the current folder for header files