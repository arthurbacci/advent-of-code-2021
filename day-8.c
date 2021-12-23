#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUM_LEFT 10
#define NUM_RIGHT 4
#define NUM_LINES 200

char segments_of_one[2];
char segments_of_three[5];
char segments_of_four[4];

int next_char(char *digitbuf, size_t num_segments) {
    switch (num_segments) {
        case 2:
            return 1;
        case 3:
            return 7;
        case 4:
            return 4;
        case 5: {
            unsigned int x = 0;
            for (size_t i = 0; i < num_segments; i++)
                if (digitbuf[i] == segments_of_one[0]
                ||  digitbuf[i] == segments_of_one[1])
                    x++;
            if (x == 2)
                return 3;
            x = 0;
            for (size_t i = 0; i < num_segments; i++) {
                bool y = 0;
                for (size_t j = 0; j < 4; j++)
                    if (digitbuf[i] == segments_of_four[j])
                        y = 1;
                x += y;
            }
            if (x == 3)
                return 5;
            return 2;
        }
        case 6: {
            unsigned int x = 0;
            for (size_t i = 0; i < num_segments; i++) {
                bool y = 0;
                for (size_t j = 0; j < 4; j++)
                    if (digitbuf[i] == segments_of_four[j])
                        y = 1;
                x += y;
            }
            if (x == 4)
                return 9;
            x = 0;
            for (size_t i = 0; i < num_segments; i++)
                if (digitbuf[i] == segments_of_one[0]
                ||  digitbuf[i] == segments_of_one[1])
                    x++;
            if (x == 2)
                return 0;
            return 6;
        }
        case 7:
            return 8;
            break;
    }
}

void find_segments(char *digitbuf, size_t num_segments) {
    switch (num_segments) {
        case 2:
            segments_of_one[0] = digitbuf[0];
            segments_of_one[1] = digitbuf[1];
            break;
        case 4:
            for (size_t i = 0; i < num_segments; i++)
                segments_of_four[i] = digitbuf[i];
            break;
        case 5: {
            unsigned int x = 0;
            for (size_t i = 0; i < num_segments; i++)
                if (digitbuf[i] == segments_of_one[0]
                ||  digitbuf[i] == segments_of_one[1])
                    x++;
            if (x == 2) {
                for (size_t i = 0; i < num_segments; i++)
                    segments_of_three[i] = digitbuf[i];
            }
            break;
        }
    }




}

int main() {
    FILE *fp = fopen("input/day-8.input", "r");

    unsigned int part1r = 0;
    for (size_t line = 0; line < NUM_LINES; line++) {
        for (size_t digit = 0; digit < NUM_LEFT; digit++) {
            char digitbuf[8];
            fscanf(fp, "%s ", digitbuf);
        }
        fgetc(fp);
        for (size_t digit = 0; digit < NUM_RIGHT; digit++) {
            char digitbuf[8];
            fscanf(fp, " %s", digitbuf);
            size_t num_segments = strlen(digitbuf);
            switch (num_segments) {
                case 2:
                case 4:
                case 3:
                case 7:
                    part1r++;
                    break;
            }
        }
        fgetc(fp);
    }

    printf("PART 1: %u\n", part1r);

    rewind(fp);

    int total = 0;
    for (size_t line = 0; line < NUM_LINES; line++) {
        for (size_t digit = 0; digit < NUM_LEFT; digit++) {
            char digitbuf[8];
            fscanf(fp, "%s ", digitbuf);
            size_t num_segments = strlen(digitbuf);
            find_segments(digitbuf, num_segments);
        }
        fgetc(fp);
        int r = 0;
        int v = 1000;
        for (size_t digit = 0; digit < NUM_RIGHT; digit++) {
            char digitbuf[8];
            fscanf(fp, " %s", digitbuf);
            size_t num_segments = strlen(digitbuf);
            r += v * next_char(digitbuf, num_segments);
            v /= 10;
        }
        total += r;
    }

    printf("PART 2: %d\n", total);

    fclose(fp);
    return 0;
}