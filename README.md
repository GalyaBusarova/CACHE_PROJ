# Cache Replacement Algorithms: LFU, LRU and Optimal Caching

A C++17 project implementing and comparing three cache replacement strategies:

- Least Frequently Used (LFU)
- Least Recently Used (LRU)
- Optimal Caching

The project includes unit tests, synthetic request-trace generation, and a benchmarking framework.

## Overview

Cache replacement policies determine which item should be evicted when a cache reaches its capacity.

This project implements three approaches with different information requirements and performance characteristics.

```text
                  Request Stream
                       |
          +------------+------------+
          |            |            |
          v            v            v
        LFU           LRU      Optimal Cache
          |            |            |
          +------------+------------+
                       |
                       v
                Cache Statistics
                       |
                       v
                 Benchmarking
```

## Implemented Algorithms

### LFU — Least Frequently Used

Evicts the item with the lowest access frequency.

The implementation maintains access-frequency information and uses it to select eviction candidates.

### LRU — Least Recently Used

Evicts the item that has not been accessed for the longest period of time.

LRU provides a useful baseline for comparison with frequency-based policies.

### Optimal Caching

Implements the theoretically optimal offline replacement strategy.

The algorithm evicts the cached item whose next request occurs farthest in the future.

Unlike practical online algorithms, optimal caching requires knowledge of the complete future request sequence and is therefore primarily useful as a theoretical benchmark.

## Why Compare These Algorithms?

The three algorithms represent different approaches to cache replacement:

| Algorithm | Information             | Practical Use             |
| --------- | ----------------------- | ------------------------- |
| LFU       | Past access frequency   | Frequency-based workloads |
| LRU       | Recent access history   | Locality-based workloads  |
| Optimal   | Future request sequence | Theoretical upper bound   |

Comparing them helps illustrate how workload characteristics influence cache hit rates.

## Workload Generation

The project includes a Python-based request trace generator.

Synthetic traces can be generated using a Zipf-like distribution to model non-uniform access patterns.

This allows experiments with different degrees of locality and skew.

## Benchmarking

The repository contains a benchmarking framework for comparing cache implementations.

The benchmark can evaluate cache behavior for different:

* cache sizes;
* request counts;
* request distributions;
* workloads.

The main metric is cache hit behavior.

## Tests

Unit tests are included for validating the LFU and optimal-cache implementations.

Run:

```bash
make tests
```

## Build

### Requirements

* C++17-compatible compiler
* `make`
* Python 3
* NumPy for trace generation

### Build

```bash
make
```

## Usage

### Interactive Mode

```bash
make run
```

The program asks for:

* cache size;
* number of requests;
* request sequence.

### Run on a Prepared Trace

```bash
make run-file TEST=your_file.dat
```

### Generate a Trace

```bash
make generate-trace
```

### Quick Benchmark

```bash
make quick-benchmark
```

### Full Benchmark

```bash
make full-benchmark
```

### Clean

```bash
make clean
```

## Project Structure

```text
CACHE_PROJ/
├── hash.h
├── ideal_cache.h
├── lru_cache.h
├── lfu.cpp
├── benchmark.cpp
├── main_benchmark.cpp
├── tests.cpp
├── tests.h
├── trace_generator.py
├── Makefile
├── tests/
└── README.md
```

## Technologies

* C++17
* Python
* NumPy
* Make
* Benchmarking
* Cache algorithms
* Unit testing

## What I Learned

This project provided practical experience with:

* cache replacement algorithms;
* algorithmic trade-offs;
* workload generation;
* cache hit behavior;
* benchmarking;
* unit testing;
* C++ data structures.

## Future Work

Possible extensions include:

* additional replacement policies;
* configurable workload distributions;
* memory and runtime profiling;
* larger-scale benchmark experiments;
* visualization of hit-rate curves;
* analysis of workload sensitivity;
* comparison with adaptive replacement policies.

## Author

Galina Busarova

Moscow Institute of Physics and Technology (MIPT)
