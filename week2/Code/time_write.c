#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);

    FILE *f = fopen("../../data/output.txt", "w");

    clock_t start = clock();

    for (int i = 0; i < n; i++) {
        fprintf(f, "%d\n", i);
    }

    clock_t end = clock();

    fclose(f);

    printf("Write time: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
