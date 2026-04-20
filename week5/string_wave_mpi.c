#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int check_args(int argc, char **argv);
void initialise_vector(double vector[], int size, double initial);
void update_positions(double* positions, int points, double time);
double driver(double time);
void print_header(FILE** p_out_file, int points);

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int points = check_args(argc, argv);

    int cycles = 5;
    int samples = 25;
    int time_steps = cycles * samples + 1;
    double step_size = 1.0 / samples;

    int local_points = points / size;

    double *time_stamps = NULL;
    double *positions = NULL;
    double *local_positions = malloc(local_points * sizeof(double));
    double *recv_positions = NULL;

    if (rank == 0)
    {
        time_stamps = malloc(time_steps * sizeof(double));
        positions = malloc(points * sizeof(double));

        initialise_vector(time_stamps, time_steps, 0.0);

        for (int i = 0; i < time_steps; i++)
        {
            time_stamps[i] = i * step_size;
        }

        initialise_vector(positions, points, 0.0);
    }

    MPI_Scatter(positions, local_points, MPI_DOUBLE,
                local_positions, local_points, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    for (int t = 0; t < time_steps; t++)
    {
        double time = t * step_size;

        update_positions(local_positions, local_points, time);

        MPI_Gather(local_positions, local_points, MPI_DOUBLE,
                   positions, local_points, MPI_DOUBLE,
                   0, MPI_COMM_WORLD);

        if (rank == 0)
        {
            FILE* out_file = fopen("data/string_wave.csv", "a");

            if (t == 0)
                print_header(&out_file, points);

            fprintf(out_file, "%d, %lf", t, time);

            for (int i = 0; i < points; i++)
            {
                fprintf(out_file, ", %lf", positions[i]);
            }

            fprintf(out_file, "\n");
            fclose(out_file);
        }
    }

    free(local_positions);

    if (rank == 0)
    {
        free(time_stamps);
        free(positions);
    }

    MPI_Finalize();
    return 0;
}
