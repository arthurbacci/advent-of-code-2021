#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int *parsenumbers(FILE *fp) {
    char *buf = NULL;
    size_t bufcap = 0;
    size_t len = getline(&buf, &bufcap, fp);

    char *nbuf = buf;

    int *ret = calloc(100, sizeof(int));

    for (size_t i = 0; i < 100; i++) {
        ret[i] = atoi(nbuf);
        while (*nbuf != '\n' && *(nbuf++) != ',');
    }

    free(buf);

    return ret;
}

int mark_next_board(FILE *fp, FILE *tmp, int number) {
    // Skips one empty line
    {
        char *buf = NULL;
        size_t bufcap = 0;
        if (getline(&buf, &bufcap, fp) == -1)
            return 0;
        free(buf);
    }

    bool c[5] = {1, 1, 1, 1, 1};
    bool row = 0;
    int number_of_unmarked = 0;
    for (size_t i = 0; i < 5; i++) {
        char *buf = NULL;
        size_t bufcap = 0;
        size_t len = getline(&buf, &bufcap, fp);

        if (len == -1)
            return 0;
        
        char *nbuf = buf;
        fputc('\n', tmp);
        bool r = 1;
        for (size_t j = 0; j < 5; j++) {
            while (*nbuf == ' ') nbuf++;
            int n = atoi(nbuf);
            if (n == number)
                fprintf(tmp, "-1");
            else {
                fprintf(tmp, "%2d", n);
                if (n != -1) {
                    r = 0;
                    c[j] = 0;
                    number_of_unmarked += n;
                }
            }
            if (j < 4)
                fprintf(tmp, " ");
            while (*nbuf != '\n' && *(nbuf++) != ' ');
        }
        if (r == 1)
            row = 1;

        free(buf);
    }
    fputc('\n', tmp);
    bool col = c[0] || c[1] || c[2] || c[3] || c[4];
    if (col || row) {
        return number_of_unmarked * number;
    }
    return 0;
}

bool wins[100] = {0};
int first = -1;
int last = -1;

void mark_all_boards(FILE *fp, FILE *tmp, int number) {
    for (size_t i = 0; i < 100; i++) {
        int s = mark_next_board(fp, tmp, number);
        if (s > 0 && !wins[i]) {
            last = s;
            if (first == -1)
                first = s;
            wins[i] = 1;
        }
    }
}

int main() {
    FILE *fp = fopen("input/day-4.input", "r");

    int *sorted_numbers = parsenumbers(fp);

    for (size_t i = 0; i < 100; i++) {
        FILE *tmp = tmpfile();
        mark_all_boards(fp, tmp, sorted_numbers[i]);
        fclose(fp);
        fp = tmp;
        rewind(fp);
    }

    free(sorted_numbers);

    printf("PART 1: %d\n", first);
    printf("PART 2: %d\n", last);


    fclose(fp);

    return 0;
}