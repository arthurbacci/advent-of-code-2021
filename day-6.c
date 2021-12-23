#include <stdio.h>
#include <stdlib.h>

#define DAYS 256

int main() {
    FILE *fp = fopen("input/day-6.input", "r");

    long fish[DAYS + 6] = {1, 1, 1, 1, 1, 1, 1, 2, 2};
    for (size_t i = 9; i < DAYS + 6; i++)
        fish[i] = fish[i - 7] + fish[i - 9];

    long fishes = 0;
    while (1) {
        fishes += fish[DAYS - (fgetc(fp) - '0') + 6];
        if (fgetc(fp) != ',')
            break;
    }
    
    printf("PART 1: %ld\n", fishes);

    fclose(fp);
    return 0;
}