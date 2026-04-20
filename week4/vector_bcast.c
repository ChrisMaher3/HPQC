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
        if (rank == 0) printf("Usage: mpirun -np <p> ./vector_bcast <N>\n");
        MPI_Finalize();
        return 0;
    }

    int N = atoi(argv[1]);
    int chunk = N / size;

    int *vec = malloc(N * sizeof(int));

    if (rank == 0)
        for (int i = 0; i < N; i++) vec[i] = i + 1;

    MPI_Bcast(vec, N, MPI_INT, 0, MPI_COMM_WORLD);

    int *local = malloc(chunk * sizeof(int));

    for (int i = 0; i < chunk; i++)
        local[i] = vec[rank * chunk + i];

    int sum = 0;
    for (int i = 0; i < chunk; i++) sum += local[i];

    int total = 0;
    MPI_Reduce(&sum, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) printf("Sum: %d\n", total);

    free(local);
    free(vec);

    MPI_Finalize();
    return 0;
}
