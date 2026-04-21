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

    double *positions = NULL;
    double *local_positions = malloc(local_points * sizeof(double));

    if (rank == 0)
    {
        positions = malloc(points * sizeof(double));
        initialise_vector(positions, points, 0.0);
    }

    MPI_Scatter(positions, local_points, MPI_DOUBLE,
                local_positions, local_points, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    FILE *out_file = NULL;

    if (rank == 0)
    {
        out_file = fopen("data/string_wave.csv", "w");
        print_header(&out_file, points);
    }

    for (int t = 0; t < time_steps; t++)
    {
        double time = t * step_size;

        update_positions(local_positions, local_points, time);

        MPI_Gather(local_positions, local_points, MPI_DOUBLE,
                   positions, local_points, MPI_DOUBLE,
                   0, MPI_COMM_WORLD);

        if (rank == 0)
        {
            fprintf(out_file, "%d, %lf", t, time);

            for (int i = 0; i < points; i++)
            {
                fprintf(out_file, ", %lf", positions[i]);
            }

            fprintf(out_file, "\n");
        }
    }

    if (rank == 0)
    {
        fclose(out_file);
        free(positions);
    }

    free(local_positions);

    MPI_Finalize();
    return 0;
}

void update_positions(double* positions, int points, double time)
{
    double* new_positions = malloc(points * sizeof(double));

    new_positions[0] = driver(time);

    for (int i = 1; i < points - 1; i++)
    {
        new_positions[i] = 0.5 * (positions[i - 1] + positions[i + 1]);
    }

    new_positions[points - 1] = 0.0;

    for (int i = 0; i < points; i++)
    {
        positions[i] = new_positions[i];
    }

    free(new_positions);
}

double driver(double time)
{
    return sin(time * 2.0 * M_PI);
}

void initialise_vector(double vector[], int size, double initial)
{
    for (int i = 0; i < size; i++)
    {
        vector[i] = initial;
    }
}

void print_header(FILE** p_out_file, int points)
{
    fprintf(*p_out_file, "#, time");
    for (int j = 0; j < points; j++)
    {
        fprintf(*p_out_file, ", y[%d]", j);
    }
    fprintf(*p_out_file, "\n");
}

int check_args(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [POINTS]\n", argv[0]);
        exit(-1);
    }

    return atoi(argv[1]);
}
