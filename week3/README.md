## Serial vs MPI Hello World Benchmark

| Type     | Processes | Real Time (s) | User Time (s) | Sys Time (s) |
|----------|----------|--------------|--------------|-------------|
| Serial   | 1        | 0.005        | 0.000        | 0.004       |
| MPI      | 2        | 0.429        | 0.100        | 0.091       |
| MPI      | 4        | 0.422        | 0.111        | 0.174       |
| MPI      | 8        | 0.444        | 0.211        | 0.295       |
| MPI      | 16       | 0.474        | 0.321        | 0.547       |

## proof.c explanation

### main()
The main function starts by setting up MPI using MPI_Init. It then reads in any command line arguments and checks they are valid. After that, it gets the rank of the current process (my_rank) and the total number of processes (uni_size). These values are used to decide what each process should do. At the end of the program MPI is closed using MPI_Finalize.

### MPI setup
MPI_Init is used to start the MPI environment so that processes can communicate with each other. Without this, none of the MPI functions would work.

### Rank and size
Each process is given a rank number (my_rank). This is basically its ID. The total number of processes is stored in uni_size. Rank 0 is usually treated as the root process, and the others are worker processes.

### check_task()
This function decides what each process does based on its rank. If the rank is 0, it acts as the root and receives data from the other processes. If the rank is greater than 0, it acts as a worker and sends its result to the root.

# Vector Serial vs MPI Benchmark Results

## Serial Implementation

| Input Size | Output | Real Time (s) | User Time (s) | Sys Time (s) |
|------------|--------|--------------|---------------|--------------|
| 1,000      | 500500        | 0.005 | 0.000 | 0.004 |
| 10,000     | 50005000      | 0.004 | 0.000 | 0.004 |
| 100,000    | 705082704     | 0.006 | 0.003 | 0.003 |
| 1,000,000  | 1784293664    | 0.019 | 0.015 | 0.004 |

## MPI Implementation (np = 2)

| Input Size | Output | Real Time (s) | User Time (s) | Sys Time (s) |
|------------|--------|--------------|---------------|--------------|
| 10,000     | 50005000      | 0.411 | 0.078 | 0.108 |
| 100,000    | 705082704     | 0.411 | 0.084 | 0.099 |
| 1,000,000  | 1784293664    | 0.420 | 0.076 | 0.128 |

## MPI Implementation (np = 4)

| Input Size | Output | Real Time (s) | User Time (s) | Sys Time (s) |
|------------|--------|--------------|---------------|--------------|
| 10,000     | 50005000      | 0.416 | 0.132 | 0.159 |
| 100,000    | 705082704     | 0.421 | 0.091 | 0.194 |
| 1,000,000  | 1784293664    | 0.455 | 0.139 | 0.193 |

## MPI Implementation (np = 8)

| Input Size | Output | Real Time (s) | User Time (s) | Sys Time (s) |
|------------|--------|--------------|---------------|--------------|
| 10,000     | 50005000      | 0.443 | 0.230 | 0.244 |
| 100,000    | 705082704     | 0.434 | 0.207 | 0.291 |
| 1,000,000  | 1784293664    | 0.446 | 0.312 | 0.257 |
