# Cache Emulator
### README.MD
---
## Description
Name: Jiang Wang

Email: jiangwang@uchicago.edu

This program is written in `C++ 11` and developed on Ubuntu 18.04 LTS. In order to compile and produce an executable, just simply go to the project directory and prompt

```
make
```

which automatically produces a executable called `./cache-sim`.

This project has two subdirectories `lib` and `src`. `lib` contains the definitions and declarations of `FIFO` and `LRU` queues. `src` includes the definitions and declarations of the components of cache, such as `CPU`, `Cache`, and `Rule` (which is the addressing rule).

---
The executable is "./cache-sim", which supports the following flags:
1. -c val, which cache size (bytes) is determined by val
2. -b val, which block size (bytes) is determined by val
3. -n val, which associativity is determined by val
4. -r str, which replacement policy is determined by str
5. -a str, which the algorithm executed is determined by str
6. -d val, which the input size is determied by val
7. -p, which prints the output the prompt
8. -f, the blocking factor of the block mxm algorithm
9. -l, includes loading data (memory reads) into the results reported

This program provides a fast way to examine the outputs:

##### 2.1 Correctness check
```
make test-daxpy
make test-mxm
make test-mxmblock
```

##### 2.2 Associativity
```
make test-associativity
```

##### 2.3 Memory Block Size
```
make test-block-size
```

##### 2.4 Total Cache Size
```
test-cache-size
```

##### 2.5 Problem Size and Cache Thrashing
```
make test-cache-thrashing-1
make test-cache-thrashing-2
make test-cache-thrashing-3
```

##### 2.6 Replacement Policy
```
make test-replacement-policy
```
