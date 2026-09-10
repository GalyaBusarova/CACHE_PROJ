# LFU, LRU and Optimal Caching

This project implements three cache replacement algorithms:
- **LFU (Least Frequently Used)** — evicts the item with the lowest access frequency.
- **LRU (Least Recentle Used)** -  evicts the item that has not been accessed for the longest time.
- **Optimal Caching** — a theoretically optimal algorithm that evicts the item whose next access is farthest in the future. It requires knowledge of the entire request sequence in advance.

The project includes implementations of all three algorithms, unit tests, and a benchmarking framework.

---

## 📁 Project Structure
```bash
├── hash.h # LFU cache implementation
├── ideal_cache.h # Ideal cache implementation and hit counting functions
├── lru_cache.h # LRU cache implementation for comparison
├── lfu.cpp # Main program
├── main_benchmark.cpp # Benchmarking program
├── benchmark.cpp # Benchmarking logic
├── main_lru.cpp # LRU program
├── trace_generator.py        # Trace generator in Python (Zipf distribution)
├── trace_generator.cpp       # Trace generator in C++ (alternative)
├── tests.cpp # Unit tests for LFU and ideal caching
├── tests.h # Declaration of structures and functions for tests
├── Makefile # Project build
└── README.md # This file
```

## 🛠 Build and Run

### Requirements
- C++17 compiler or higher (e.g., g++, clang++)
- `make`
- Python 3 with numpy for the trace generator

### Build
```bash
make
```

This will build all programs: implementations, tests, and benchmark.

### Run

1. Interactive mode
```bash
make run
```

The program will prompt for: cache size, number of requests, sequence of numbers (keys)

2. Run on a prepared test file
Place the test file in the project root and run:

```bash
make run-file TEST=your_file.dat
```

3. Run unit tests

```bash
make tests
```

4. Benchmarking: LRU vs LFU
Generate a test trace:
```bash
make generate-trace
```

Quick benchmark (cache size = 1000 only):
```bash
make quick-benchmark
```

Full benchmark
```bash
make full-benchmark
```

### Cleanup
```bash
make clean
```



