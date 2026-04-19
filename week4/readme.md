## MPI Communication Test

The program comm_test_mpi.c was compiled and run using different numbers of processes.

### Observations

For 2 processes, the behaviour is simple: rank 1 sends a value to rank 0, and rank 0 receives it.

For 4 and 8 processes, multiple ranks send data to rank 0. The order in which the "Sent" messages appear is not sequential and can vary between runs. For example, in the 8-process run, ranks 6, 3, 4, 5, and 7 printed before rank 1.

This shows that MPI processes execute independently and asynchronously. There is no guaranteed order for when each process runs or sends its message.

However, rank 0 still receives all messages correctly, showing that MPI communication works even if execution order is not predictable.

## Functionalise the code

The modified program was compiled and run again with multiple process counts (2, 4, and 8), and produced the same output as before, confirming that the refactoring did not change the behaviour of the program.

## MPI Send Variants

Different send functions were tested by modifying the communication code.

MPI_Send worked as expected and produced consistent results.

MPI_Ssend (synchronous send) also worked correctly but is a blocking operation, meaning the sender waits until the receiver is ready. This can make it slower but more predictable.

MPI_Isend (non-blocking send) required the use of MPI_Wait to ensure the message was completed. Without this, the program behaved incorrectly.

MPI_Rsend was not reliable and caused errors if the receive is not already posted.

## MPI Communication Timing

The program was executed using the terminal time command with multiple processes.

Example run with 4 processes:

real    0m0.445s 
user    0m0.112s 
sys     0m0.174s 

The results show that execution time is very short and remains relatively consistent across runs. Some variation is expected due to process scheduling and MPI communication overhead.

This demonstrates that MPI communication for small messages is fast, but timing measurements at this scale can vary slightly between runs.
