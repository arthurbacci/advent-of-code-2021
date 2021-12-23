#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 2000
#define HEIGHT 2000

#define POS(x, y) ((y) * (WIDTH) + (x))

int main() {
    FILE *fp = fopen("input/day-13.input", "r");

    bool grid[WIDTH * HEIGHT];
    for (size_t i = 0; i < WIDTH * HEIGHT; i++)
        grid[i] = 0;
    size_t width = WIDTH;
    size_t height = HEIGHT;

    while (1) {
        size_t x, y;
        if (fscanf(fp, "%lu,%lu\n", &x, &y) == 2)
            grid[POS(x, y)] = 1;
        else
            break;
    }

    unsigned int part1 = 0;
    while (1) {
        for (size_t i = 0; i < 11; i++)
            if (fgetc(fp) == EOF)
                goto END;

        char dir = fgetc(fp);
        fgetc(fp);

        size_t pos;
        fscanf(fp, "%lu", &pos);
        fgetc(fp);

        bool newgrid[WIDTH * HEIGHT];
        for (size_t i = 0; i < WIDTH * HEIGHT; i++)
            newgrid[i] = 0;

        if (dir == 'x') {
            for (size_t y = 0; y < HEIGHT; y++) {
                for (size_t x = 0; x < pos; x++) {
                    newgrid[POS(x, y)] = grid[POS(x, y)];
                    if (2 * pos - x < WIDTH)
                        newgrid[POS(x, y)] += grid[POS(2 * pos - x, y)];
                }
            }
            width = pos;
        } else if (dir == 'y') {
            for (size_t y = 0; y < pos; y++) {
                for (size_t x = 0; x < WIDTH; x++) {
                    newgrid[POS(x, y)] = grid[POS(x, y)];
                    if (2 * pos - y < HEIGHT)
                        newgrid[POS(x, y)] += grid[POS(x, 2 * pos - y)];
                }
            }
            height = pos;
        }

        for (size_t i = 0; i < WIDTH * HEIGHT; i++)
            grid[i] = newgrid[i];

        if (part1 == 0) {
            for (size_t y = 0; y < HEIGHT; y++) {
                for (size_t x = 0; x < WIDTH; x++) {
                    if (grid[POS(x, y)])
                        part1++;
                }
            }
        }
    }
    END: {}


    printf("PART 1: %u\n", part1);
    printf("PART 2:\n");

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            putchar(grid[POS(x, y)] ? '#' : '.');
        }
        putchar('\n');
    }

    fclose(fp);
    return 0;
}