#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 100
#define HEIGHT 100
#define NUM_GRIDS 5

#define  POS(w, h) ((h) * WIDTH             + (w))
#define NPOS(w, h) ((h) * WIDTH * NUM_GRIDS + (w))

size_t next(size_t pos, size_t *ret, size_t width, size_t height) {
    size_t x = pos % width;
    size_t y = pos / width;

    size_t ret_sz = 0;

    if (x > 0)
        ret[ret_sz++] = pos - 1;
    if (x + 1 < width)
        ret[ret_sz++] = pos + 1;
    if (y > 0)
        ret[ret_sz++] = pos - width;
    if (y + 1 < height)
        ret[ret_sz++] = pos + width;
    
    return ret_sz;
}

void read_grid(char *grid, FILE *fp) {
    for (size_t h = 0; h < HEIGHT; h++) {
        for (size_t w = 0; w < WIDTH; w++) {
            char c = fgetc(fp);
            grid[POS(w, h)] = c - 48;
        }
        fgetc(fp); // \n
    }
}

unsigned long dijkstra(char *grid, size_t width, size_t height) {
    size_t pos = 0;

    bool qset[width * height];
    unsigned long risk[width * height];
    ssize_t prev[width * height];
    for (size_t i = 0; i < width * height; i++) {
        qset[i] = 1;
        risk[i] = 0 - 1;
        prev[i] = -1;
    }

    risk[pos] = 0;

    while (1) {
        bool q_empty = 1;
        size_t min = 0;
        for (size_t i = 0; i < width * height; i++) {
            if (qset[i]) {
                q_empty = 0;
                if (!qset[min] || risk[i] < risk[min])
                    min = i;
            }
        }

        if (q_empty)
            break;

        qset[min] = 0;


        size_t neighbours[4];
        for (size_t i = 0; i < next(min, neighbours, width, height); i++) {
            size_t v = neighbours[i];
            if (qset[v]) {
                unsigned long alt = risk[min] + grid[v];
                if (alt < risk[v]) {
                    risk[v] = alt;
                    prev[v] = min;
                }
            }
        }
    }

    return risk[width * height - 1];
}

int main() {
    FILE *fp = fopen("input/day-15.input", "r");
    char grid[WIDTH * HEIGHT];
    read_grid(grid, fp);
    fclose(fp);

    printf("PART 1: %lu\n", dijkstra(grid, WIDTH, HEIGHT));

    char newgrid[WIDTH * HEIGHT * NUM_GRIDS * NUM_GRIDS];
    for (size_t hh = 0; hh < NUM_GRIDS; hh++)
        for (size_t ww = 0; ww < NUM_GRIDS; ww++)
            for (size_t h = 0; h < HEIGHT; h++)
                for (size_t w = 0; w < WIDTH; w++)
                    newgrid[NPOS(w + WIDTH * ww, h + HEIGHT * hh)] =
                        (grid[POS(w, h)] + ww + hh - 1) % 9 + 1;
    
    printf("PART 2: %lu\n",
           dijkstra(newgrid, WIDTH * NUM_GRIDS, HEIGHT * NUM_GRIDS));


    return 0;
}