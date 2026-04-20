#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("Usage: ./string_wave cycles samples output_file\n");
        return 1;
    }

    int cycles = atoi(argv[1]);
    int samples = atoi(argv[2]);
    char *outfile = argv[3];

    FILE *fp = fopen(outfile, "w");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    double *string = malloc(samples * sizeof(double));
    double *new_string = malloc(samples * sizeof(double));

    for (int i = 0; i < samples; i++)
    {
        string[i] = sin((double)i / samples * M_PI);
    }

    for (int t = 0; t < cycles; t++)
    {
        for (int i = 1; i < samples - 1; i++)
        {
            new_string[i] = 0.5 * (string[i - 1] + string[i + 1]);
        }

        for (int i = 1; i < samples - 1; i++)
        {
            string[i] = new_string[i];
        }

        for (int i = 0; i < samples; i++)
        {
            fprintf(fp, "%f,", string[i]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    free(string);
    free(new_string);

    return 0;
}
