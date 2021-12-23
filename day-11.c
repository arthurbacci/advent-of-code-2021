#include <stdio.h>
#include <stdlib.h>

#define WIDTH 10
#define HEIGHT 10

#define SIZE(x, y) ((x) * (y))
#define POS(x, y) ((y) * (WIDTH) + (x))

unsigned int flash(size_t w, size_t h, char *grid) {
    unsigned int flashes = 1;

    grid[POS(w, h)] = ':';

    for (size_t he = h > 0 ? h - 1 : 0;
         he <= (h + 1 < HEIGHT ? h + 1 : h);
         he++) {
        for (size_t wi = w > 0 ? w - 1 : 0;
             wi <= (w + 1 < WIDTH ? w + 1 : w);
             wi++) {
            if (wi != w || he != h) {
                char v = grid[POS(wi, he)];
                if (v == '9')
                    flashes += flash(wi, he, grid);
                else if (v != ':')
                    grid[POS(wi, he)]++;
            }
        }
    }

    return flashes;
}

unsigned int step(char *grid) {
    unsigned int flashes = 0;
    for (size_t h = 0; h < HEIGHT; h++) {
        for (size_t w = 0; w < WIDTH; w++) {
            char m = grid[POS(w, h)];

            if (m == '9')
                flashes += flash(w, h, grid);
            else if (m != ':')
                grid[POS(w, h)]++;
        }
    }

    for (size_t i = 0; i < SIZE(WIDTH, HEIGHT); i++)
        if (grid[i] == ':')
            grid[i] = '0';

    return flashes;
}

int main() {
    FILE *fp = fopen("input/day-11.input", "r");

    char grid[SIZE(WIDTH, HEIGHT)];

    for (size_t h = 0; h < HEIGHT; h++) {
        for (size_t w = 0; w < WIDTH; w++)
            grid[POS(w, h)] = fgetc(fp);
        fgetc(fp);
    }


    unsigned int flashes = 0;
    size_t s = 0;
    for (size_t i = 0; i < 1000; i++) {
        unsigned int f = step(grid);
        flashes += f;
        if (f == SIZE(WIDTH, HEIGHT) && s == 0)
            s = i + 1;
    }

    printf("PART 1: %u\n", flashes);
    printf("PART 2: %lu\n", s);
    
    for (size_t h = 0; h < HEIGHT; h++) {
        for (size_t w = 0; w < WIDTH; w++) {
            printf("%c", grid[POS(w, h)]);
        }
        printf("\n");
    }


    fclose(fp);
    return 0;
}