## MPI Communication Test

The program comm_test_mpi.c was compiled and run using different numbers of processes.

### Observations

For 2 processes, the behaviour is simple: rank 1 sends a value to rank 0, and rank 0 receives it.

For 4 and 8 processes, multiple ranks send data to rank 0. The order in which the "Sent" messages appear is not sequential and can vary between runs. For example, in the 8-process run, ranks 6, 3, 4, 5, and 7 printed before rank 1.

This shows that MPI processes execute independently and asynchronously. There is no guaranteed order for when each process runs or sends its message.

However, rank 0 still receives all messages correctly, showing that MPI communication works even if execution order is not predictable.
