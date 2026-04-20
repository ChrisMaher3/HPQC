#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank, size;
    int num_pings, array_size;
    int *data;
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

    if (argc != 3)
    {
        if (rank == 0)
            printf("Usage: mpirun -np 2 ./pingpong_bandwidth <pings> <array_size>\n");
        MPI_Finalize();
        return 0;
    }

    num_pings = atoi(argv[1]);
    array_size = atoi(argv[2]);

    data = malloc(array_size * sizeof(int));

    for (int i = 0; i < array_size; i++)
        data[i] = i;

    start = MPI_Wtime();

    for (int i = 0; i < num_pings; i++)
    {
        if (rank == 0)
        {
            MPI_Send(data, array_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(data, array_size, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        else
        {
            MPI_Recv(data, array_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(data, array_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    end = MPI_Wtime();

    if (rank == 0)
    {
        printf("Size: %d ints\n", array_size);
        printf("Elapsed time: %f\n", end - start);
        printf("Average time: %f\n", (end - start) / num_pings);
    }

    free(data);
    MPI_Finalize();
    return 0;
}
