# Week 2


## C Repeat Adder Performance

| Input (a × b=3) | Output | Real Time (s) | User Time (s) | Sys Time (s) |
|----------------|--------|--------------|--------------|-------------|
| 2 × 3          | 6      | 0.005        | 0.000        | 0.004       |
| 10 × 3         | 30     | 0.004        | 0.000        | 0.004       |
| 100 × 3        | 300    | 0.004        | 0.004        | 0.000       |
| 1000 × 3       | 3000   | 0.003        | 0.000        | 0.003       |
| 10000 × 3      | 30000  | 0.004        | 0.004        | 0.000       |

## Python Repeat Adder Performance

| Input (a × b=3) | Output | Real Time (s) | User Time (s) | Sys Time (s) |
|----------------|--------|--------------|--------------|-------------|
| 2 × 3          | 6      | 0.028        | 0.020        | 0.008       |
| 10 × 3         | 30     | 0.033        | 0.024        | 0.008       |
| 100 × 3        | 300    | 0.030        | 0.017        | 0.013       |
| 1000 × 3       | 3000   | 0.028        | 0.020        | 0.008       |
| 10000 × 3      | 30000  | 0.028        | 0.016        | 0.012       |

Overall, these results demonstrate the impact of language implementation (compiled vs interpreted) on execution performance.

## Internal Timing (time_print)

### C Implementation

| Input | Runtime (s) |
|------|------------|
| 10       | 0.000162 |
| 100      | 0.000183 |
| 1000     | 0.000423 |
| 10000    | 0.001935 |
| 100000   | 0.021019 |

### Python Implementation

| Input | Runtime (s) |
|------|------------|
| 10       | 0.00001335 |
| 100      | 0.00006175 |
| 1000     | 0.00055146 |
| 10000    | 0.00726223 |
| 100000   | 0.07441783 |

## File I/O Performance Comparison

| Size | C Write (s) | C Read (s) | Python Write (s) | Python Read (s) |
|------|-------------|------------|------------------|-----------------|
| 1,000   | 0.000033 | 0.000048 | 0.000315 | 0.0000989 |
| 10,000  | 0.000432 | 0.000471 | 0.001549 | 0.000396 |
| 50,000  | 0.001980 | 0.001765 | 0.006994 | 0.002182 |

## Overall Conclusions

Across all experiments, C consistently outperforms Python in both computation and file I/O tasks. This is due to C being a compiled language with lower runtime overhead, while Python is interpreted and incurs additional processing costs.

The repeat adder experiments show that execution time scales with input size, but Python has consistently higher overhead even for simple arithmetic operations.

The internal timing experiments provide a clearer view of computational scaling by isolating core execution time, removing external system overhead.

File I/O experiments demonstrate that disk operations significantly increase runtime compared to pure computation. Writing is generally slower than reading, and Python exhibits higher overhead in both cases.

