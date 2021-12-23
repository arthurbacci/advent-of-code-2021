#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int quickselect(unsigned int *numbers, unsigned int selected_num) {
    unsigned int base[1000];
    memcpy(base, numbers, sizeof(unsigned int) * 1000);
    size_t base_sz = 1000;


    while (1) {
        unsigned int pivot = *base;
        unsigned int left[1000], middle[1000], right[1000];
        size_t left_sz = 0;
        size_t middle_sz = 0;
        size_t right_sz = 0;

        for (size_t i = 0; i < base_sz; i++) {
            if (base[i] < pivot)
                left[left_sz++] = base[i];
            else if (base[i] > pivot)
                right[right_sz++] = base[i];
            else
                middle[middle_sz++] = base[i];
        }

        if (selected_num < left_sz) {
            base_sz = left_sz;
            memcpy(base, left, sizeof(unsigned int) * base_sz);
        } else if (selected_num < left_sz + middle_sz) {
            return pivot;
        } else {
            selected_num -= left_sz + middle_sz;
            base_sz = right_sz;
            memcpy(base, right, sizeof(unsigned int) * base_sz);
        }
    }
}

unsigned int triangle(unsigned int x) {
    return x * (x + 1) / 2;
}

int main() {

    FILE *fp = fopen("input/day-7.input", "r");

    unsigned int numbers[1000];

    for (size_t i = 0; i < 1000; i++) {
        unsigned int n;
        fscanf(fp, "%u\n", &n);
        numbers[i] = n;
        fgetc(fp);
    }

    unsigned int pos = ( quickselect(numbers, 499)
                       + quickselect(numbers, 500) ) / 2;

    unsigned int fuel = 0;
    for (size_t i = 0; i < 1000; i++)
        fuel += numbers[i] > pos ? numbers[i] - pos : pos - numbers[i];
    
    printf("PART 1: %u\n", fuel);

    unsigned int min_fuel = 0 - 1;
    for (size_t j = 0; j < 1000; j++) {
        fuel = 0;
        for (size_t i = 0; i < 1000; i++)
            fuel += triangle(numbers[i] > j ? numbers[i] - j : j - numbers[i]);
        if (fuel < min_fuel)
            min_fuel = fuel;
    }
    
    printf("PART 2: %u\n", min_fuel);


    fclose(fp);
    return 0;
}