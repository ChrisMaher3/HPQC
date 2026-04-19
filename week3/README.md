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

