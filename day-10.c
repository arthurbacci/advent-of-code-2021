#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long quickselect(unsigned long *numbers,
                         size_t num_numbers,
                         size_t selected_num) {
    unsigned long *base = malloc(num_numbers * sizeof(*base));
    memcpy(base, numbers, sizeof(*base) * num_numbers);
    size_t base_sz = num_numbers;


    while (1) {
        unsigned long pivot = *base;
        unsigned long *left = malloc(num_numbers * sizeof(*left));
        unsigned long *middle = malloc(num_numbers * sizeof(*middle));
        unsigned long *right = malloc(num_numbers * sizeof(*right));
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
            memcpy(base, left, sizeof(*base) * base_sz);
        } else if (selected_num < left_sz + middle_sz) {
            free(left);
            free(middle);
            free(right);
            free(base);
            return pivot;
        } else {
            selected_num -= left_sz + middle_sz;
            base_sz = right_sz;
            memcpy(base, right, sizeof(*base) * base_sz);
        }
        free(left);
        free(middle);
        free(right);
    }
    free(base);
}

int main() {
    FILE *fp = fopen("input/day-10.input", "r");
    
    char closing[256];
    closing['('] = ')';
    closing['['] = ']';
    closing['{'] = '}';
    closing['<'] = '>';
    unsigned int points[256];
    points[')'] = 3;
    points[']'] = 57;
    points['}'] = 1197;
    points['>'] = 25137;

    char *stack = NULL;
    size_t stack_cap = 0;
    size_t stack_sz = 0;

    unsigned long *scores = NULL;
    size_t num_scores = 0;
    
    unsigned int tot = 0;
    char c = 0;
    while (c != EOF) {
        stack_sz = 0;
        while ((c = fgetc(fp)) != '\n' && c != EOF) {
            if (c == '(' || c == '[' || c == '{' || c == '<') {
                stack_sz++;
                if (stack_sz > stack_cap) {
                    stack_cap = stack_sz;
                    stack = realloc(stack, stack_cap * sizeof(char));
                }
                stack[stack_sz - 1] = c;
            } else {
                if (stack_sz == 0)
                    goto WRONG;
                stack_sz--;
                if (closing[stack[stack_sz]] == c)
                    continue;
                WRONG: {
                    tot += points[c];
                    goto END;
                }
            }
        }

        unsigned long score = 0;
        for (; stack_sz > 0; stack_sz--) {
            char x = stack[stack_sz - 1];
            score *= 5;
            score += x == '(' ? 1 :
                     x == '[' ? 2 :
                     x == '{' ? 3 : 4;
        }
        if (score) {
            scores = realloc(scores, (++num_scores) * sizeof(*scores));
            scores[num_scores - 1] = score;
        }
        END:
            while (c != '\n' && c != EOF)
                c = fgetc(fp);
    }
    free(stack);

    printf("PART 1: %u\n", tot);

    printf("PART 2: %u\n", quickselect(scores, num_scores, num_scores / 2));

    fclose(fp);
    free(scores);
    return 0;
}