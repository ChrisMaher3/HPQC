#include <stdio.h>
#include <time.h>

int main() {
    FILE *f = fopen("../../data/output.txt", "r");

    int x;

    clock_t start = clock();

    while (fscanf(f, "%d", &x) != EOF) {
        // reading file
    }

    clock_t end = clock();

    fclose(f);

    printf("Read time: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
