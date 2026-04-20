## Removing Hard-Coded Values

The original string wave simulation contained hard-coded values for the number of cycles, number of samples, and output file path. These values were modified so that they are now provided via command-line arguments.

The program now takes three arguments:
- Number of cycles
- Number of samples
- Output file path

This improves flexibility by allowing different simulations to be run without modifying the source code.

Example usage:
./bin/string_wave 200 50 data/output.csv

The program was tested with multiple input values and successfully generated CSV files that were visualised using the provided Python animation script.

## Parallel Strategy

The string is updated over a number of time steps, where each point depends on its neighbouring points. Because each time step depends on the previous one, the loop over time (cycles) has to stay sequential. However, the work done on each point in the string at a single time step can be split up. The array can be divided into chunks, and each MPI process works on its own section of the string. Each process calculates the new values for its part of the array independently. The only issue is at the edges of each chunk, where processes need to share boundary values with their neighbouring processes. This is done using MPI communication so each process can access the values it needs from adjacent chunks.

## Aggregation Strategy

It is not safe for multiple MPI processes to write to the same file at the same time, so the results need to be combined in a controlled way. The approach is to have each process compute its own part of the simulation, and then gather the results back to the root process. The root process is then responsible for writing everything to the output file. This can be done using MPI_Gather or MPI_Reduce depending on the type of data. For this program, gathering the full array to the root process is the most straightforward approach, and avoids file conflicts between processes. This method is also consistent with previous benchmarking work, where the root process handles output after all parallel computation is complete.
