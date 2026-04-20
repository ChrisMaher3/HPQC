#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2)
    {
        if (rank == 0) printf("Usage: mpirun -np <p> ./vector_reduce <N>\n");
        MPI_Finalize();
        return 0;
    }

    int N = atoi(argv[1]);
    int chunk = N / size;

    int *vec = NULL;
    int *local = malloc(chunk * sizeof(int));

    if (rank == 0)
    {
        vec = malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) vec[i] = i + 1;

        for (int p = 1; p < size; p++)
            MPI_Send(vec + p * chunk, chunk, MPI_INT, p, 0, MPI_COMM_WORLD);

        for (int i = 0; i < chunk; i++)
            local[i] = vec[i];
    }
    else
    {
        MPI_Recv(local, chunk, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    int sum = 0;
    for (int i = 0; i < chunk; i++)
        sum += local[i];

    int total = 0;

    MPI_Reduce(&sum, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("Sum: %d\n", total);

    free(local);
    if (rank == 0)
        free(vec);

    MPI_Finalize();
    return 0;
}
