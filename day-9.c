#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 100
#define HEIGHT 100

#define SIZE(x, y) ((x) * (y))
#define POS(x, y) ((y) * (WIDTH) + (x))


unsigned int get_basin(size_t pos_x, size_t pos_y, char *grid, bool *marked) {
    size_t pos = POS(pos_x, pos_y);
    if (marked[pos])
        return 0;
    
    unsigned int result = 1;
    marked[pos] = 1;

    char m = grid[pos];
    char l = pos_x > 0 ? grid[POS(pos_x - 1, pos_y)] : '9';
    char r = pos_x < WIDTH - 1 ? grid[POS(pos_x + 1, pos_y)] : '9';
    char t = pos_y > 0 ? grid[POS(pos_x, pos_y - 1)] : '9';
    char d = pos_y < HEIGHT - 1 ? grid[POS(pos_x, pos_y + 1)] : '9';

    if (l != '9' && l > m)
        result += get_basin(pos_x - 1, pos_y, grid, marked);
    if (r != '9' && r > m)
        result += get_basin(pos_x + 1, pos_y, grid, marked);
    if (t != '9' && t > m)
        result += get_basin(pos_x, pos_y - 1, grid, marked);
    if (d != '9' && d > m)
        result += get_basin(pos_x, pos_y + 1, grid, marked);
    return result;
}

int main() {
    FILE *fp = fopen("input/day-9.input", "r");

    char grid[SIZE(WIDTH, HEIGHT)];
    bool marked[SIZE(WIDTH, HEIGHT)];
    for (size_t i = 0; i < sizeof(marked) / sizeof(bool); i++)
        marked[i] = 0;

    for (size_t h = 0; h < HEIGHT; h++) {
        for (size_t w = 0; w < WIDTH; w++)
            grid[POS(w, h)] = fgetc(fp);
        fgetc(fp);
    }

    size_t *positions = NULL;
    size_t num_positions = 0;

    unsigned int sum = 0;
    for (size_t h = 0; h < HEIGHT; h++) {
        for (size_t w = 0; w < WIDTH; w++) {
            char m = grid[POS(w, h)];
            char l = w > 0 ? grid[POS(w - 1, h)] : ':';
            char r = w + 1 < WIDTH ? grid[POS(w + 1, h)] : ':';
            char t = h > 0 ? grid[POS(w, h - 1)] : ':';
            char d = h + 1 < HEIGHT ? grid[POS(w, h + 1)] : ':';

            if (m < l && m < r && m < t && m < d) {
                sum += m - '/';

                positions = realloc(positions,
                                    (++num_positions) * sizeof(size_t));
                positions[num_positions - 1] = POS(w, h);
            }
        }
    }

    printf("PART 1: %u\n", sum);

    unsigned int greatest[3] = {0, 0, 0};
    for (size_t i = 0; i < num_positions; i++) {
        size_t x = positions[i] % WIDTH;
        size_t y = positions[i] / WIDTH;
        unsigned int b = get_basin(x, y, grid, marked);
        if (b > greatest[0]) {
            greatest[2] = greatest[1];
            greatest[1] = greatest[0];
            greatest[0] = b;
        } else if (b > greatest[1]) {
            greatest[2] = greatest[1];
            greatest[1] = b;
        } else if (b > greatest[2])
            greatest[2] = b;
    }

    printf("PART 2: %d\n", greatest[0] * greatest[1] * greatest[2]);

    free(positions);
    fclose(fp);
    return 0;
}