#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int rank, size;
    int num_pings;
    int counter = 0;
    int value;
    double start, end;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2)
    {
        if (rank == 0)
            printf("Run with exactly 2 processes\n");
        MPI_Finalize();
        return 0;
    }

    if (argc != 2)
    {
        if (rank == 0)
            printf("Usage: mpirun -np 2 ./pingpong <num_pings>\n");
        MPI_Finalize();
        return 0;
    }

    num_pings = atoi(argv[1]);

    start = MPI_Wtime();

    while (counter < num_pings)
    {
        if (rank == 0)
        {
            MPI_Send(&counter, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        else
        {
            MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            value++;
            MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }

        if (rank == 0)
            counter++;
    }

    end = MPI_Wtime();

    if (rank == 0)
    {
        printf("Pings: %d\n", num_pings);
        printf("Elapsed time: %f\n", end - start);
        printf("Average time: %f\n", (end - start) / num_pings);
    }

    MPI_Finalize();
    return 0;
}
