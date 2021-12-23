#include <stdio.h>
#include <stdlib.h>

enum Direction {
    UP = -1,
    FORWARD = 0,
    DOWN = 1
};

struct Instruction {
    enum Direction direction;
    int amount;
};

const struct Instruction invalid_instruction = {FORWARD, 0};

struct Instruction get_instruction_from_file(FILE *fp) {
    char *buf = NULL;
    size_t bufcap = 0;
    size_t len = getline(&buf, &bufcap, fp);

    if (len != -1 && buf[len - 1] == '\n')
        buf[--len] = '\0';
    
    if (len == -1 || len == 0) {
        free(buf);
        return invalid_instruction;
    }
    enum Direction dir;

    switch (buf[0]) {
        case 'u':
            dir = UP;
            break;
        case 'f':
            dir = FORWARD;
            break;
        case 'd':
            dir = DOWN;
            break;
    }

    char *nbuf = buf;

    do {
        nbuf++;
    } while (nbuf[0] != ' ');
    
    int n = atoi(nbuf);

    free(buf);

    struct Instruction ret = {dir, n};
    return ret;
}

int main() {
    FILE *fp = fopen("input/day-2.input", "r");

    int depth = 0;
    int hpos = 0;

    while (1) {
        struct Instruction i = get_instruction_from_file(fp);
        if (i.amount == 0)
            break;
        depth += i.direction * i.amount;
        hpos += (i.direction == 0) * i.amount;
    }

    printf("PART 1: %d\n", depth * hpos);

    depth = 0;
    hpos = 0;
    int aim = 0;

    rewind(fp);

    while (1) {
        struct Instruction i = get_instruction_from_file(fp);
        if (i.amount == 0)
            break;
        aim += i.direction * i.amount;
        hpos += (i.direction == 0) * i.amount;
        depth += (i.direction == 0) * aim * i.amount;
    }

    printf("PART 2: %d\n", depth * hpos);


    fclose(fp);
    return 0;
}
