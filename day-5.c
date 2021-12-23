#include <stdio.h>
#include <stdlib.h>

#define MATRIX_SIZE 1000

int main() {
    FILE *fp = fopen("input/day-5.input", "r");

    int *matrix1 = calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(*matrix1));
    int *matrix2 = calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(*matrix2));

    for (size_t i = 0; i < 500; i++) {
        int x1, y1, x2, y2;
        fscanf(fp, "%d,%d -> %d,%d\n", &x1, &y1, &x2, &y2);
        //printf("%d %d %d %d\n", x1, y1, x2, y2);

        if (x1 == x2 || y1 == y2) {
            int s, e;
            if (x1 < x2) {
                s = x1;
                e = x2;
            } else if (x2 < x1) {
                s = x2;
                e = x1;
            } else if (y1 < y2) {
                s = y1;
                e = y2;
            } else if (y2 < y1) {
                s = y2;
                e = y1;
            }

            for (; s <= e; s++) {
                if (y1 == y2) {
                    matrix1[MATRIX_SIZE * y1 + s]++;
                    matrix2[MATRIX_SIZE * y1 + s]++;
                } else {
                    matrix1[MATRIX_SIZE * s + x1]++;
                    matrix2[MATRIX_SIZE * s + x1]++;
                }
            }
        } else {
            int xd = x1 < x2 ? 1 : -1;
            int yd = y1 < y2 ? 1 : -1;

            matrix2[MATRIX_SIZE * y1 + x1]++;
            while (x1 != x2) {
                x1 += xd;
                y1 += yd;
                matrix2[MATRIX_SIZE * y1 + x1]++;
            }
        }
    }

    unsigned int ret = 0;
    for (size_t i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
        if (matrix1[i] > 1)
            ret++;
    }

    printf("PART 1: %u\n", ret);

    ret = 0;
    for (size_t i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
        if (matrix2[i] > 1)
            ret++;
    }

    printf("PART 2: %u\n", ret);

    free(matrix1);
    free(matrix2);

    fclose(fp);
    return 0;
}