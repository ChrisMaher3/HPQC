#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

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
            printf("Run with 2 processes only\n");
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

    while (1)
    {
        if (rank == 0)
        {
            if (counter >= num_pings)
            {
                int stop = -1;
                MPI_Send(&stop, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
                break;
            }

            MPI_Send(&counter, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            counter++;
        }
        else
        {
            MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (value < 0)
                break;

            value++;
            MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
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
