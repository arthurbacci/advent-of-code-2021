#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_STEPS 40
#define STEPS (NUM_STEPS + 1)

char rules[26 * 26];

unsigned long mem[STEPS * 26 * 26 * 27];
#define MPOS(d, a, b, c) ( (d) * 26 * 26 * 27 \
                         + (a)      * 26 * 27 \
                         + (b)           * 27 \
                         + (c)                \
                         )
#define C(x) ((x) - 65)

void count(char a, char b, unsigned int d) {
    if (mem[MPOS(d, C(a), C(b), 0)])
        return;
    mem[MPOS(d, C(a), C(b), 0)] = 1;

    char x = rules[C(a) * 26 + C(b)];

    if (d == STEPS - 1) {
        mem[MPOS(d, C(a), C(b), C(a) + 1)] = 1;
        return;
    }

    count(a, x, d + 1);
    count(x, b, d + 1);

    for (size_t i = 1; i <= 26; i++) {
        mem[MPOS(d, C(a), C(b), i)] = mem[MPOS(d + 1, C(a), C(x), i)]
                                    + mem[MPOS(d + 1, C(x), C(b), i)];
    }
}

unsigned long result(unsigned int steps, char *polymer, size_t polymer_len) {
    unsigned long amount[26];
    for (size_t i = 0; i < 26; i++)
        amount[i] = 0;

    for (size_t i = 0; i < polymer_len - 1; i++) {
        char a = polymer[i];
        char b = polymer[i + 1];

        count(a, b, NUM_STEPS - steps);
        for (size_t i = 0; i < 26; i++)
            amount[i] += mem[MPOS(NUM_STEPS - steps, C(a), C(b), i + 1)];
    }
    amount[C(polymer[polymer_len - 1])]++;

    unsigned long min = 0 - 1;
    unsigned long max = 0;

    for (size_t i = 0; i < 26; i++)
        if (amount[i]) {
            if (amount[i] > max)
                max = amount[i];
            if (amount[i] < min)
                min = amount[i];
        }
    
    return max - min;
}

int main() {
    FILE *fp = fopen("input/day-14.input", "r");
    
    char *polymer = NULL;
    size_t polymer_len;

    {
        size_t t = 0;
        polymer_len = getline(&polymer, &t, fp) - 1;
    }
    

    while (1) {
        char rule[3];
        if (fscanf(fp, "\n%2c -> %c", rule, rule + 2) != 2)
            break;
        rules[C(rule[0]) * 26 + C(rule[1])] = rule[2];
    }

    printf("PART 1: %lu\n", result(10, polymer, polymer_len));
    printf("PART 2: %lu\n", result(40, polymer, polymer_len));

    free(polymer);
    fclose(fp);
    return 0;
}