#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool *get_binary_from_file(FILE *fp) {
    char *buf = NULL;
    size_t bufcap = 0;
    size_t len = getline(&buf, &bufcap, fp);

    if (len != 13) {
        free(buf);
        return NULL;
    }

    bool *ret = calloc(12, sizeof(bool));
    for (size_t i = 0; i < 12; i++)
        if (buf[i] == '1')
            ret[i] = 1;

    free(buf);

    return ret;
}

char *binary_to_string(bool *b) {
    char *ret = malloc(13);
    for (size_t i = 0; i < 12; i++)
        ret[i] = b[i] ? '1' : '0';
    ret[12] = '\0';
    return ret;
}

FILE *bit_criteria(bool common, bool most, FILE *fp) {
    for (size_t i = 0; i < 12; i++) {
        FILE *tmp = tmpfile();
        size_t ones = 0, zeroes = 0;
        size_t j = 0;;
        while (1) {
            bool *n = get_binary_from_file(fp);
            if (!n)
                break;
            
            if (n[i] == common) {
                char *str = binary_to_string(n);
                fprintf(tmp, "%s\n", str);
                free(str);
                j++;

                if (i < 11) {
                    if (n[i + 1])
                        ones++;
                    else
                        zeroes++;
                }
            }

            free(n);
        }
        if (most)
            common = ones >= zeroes;
        else
            common = ones < zeroes;
        fclose(fp);
        fp = tmp;
        rewind(fp);
        if (j == 1)
            break;
    }

    return fp;
}

unsigned short binary_to_number(bool *b, bool f) {
    unsigned short ret = 0;
    unsigned int multiplier = 1;
    for (size_t i = 0; i < 12; i++) {
        ret += b[(11 - i)] * multiplier;
        multiplier *= 2;
    }
    if (f)
        free(b);
    return ret;
}

int main() {
    FILE *fp = fopen("input/day-3.input", "r");

    int number_of_1[12] = {0};
    int number_of_0[12] = {0};

    while (1) {
        bool *n = get_binary_from_file(fp);
        if (!n)
            break;

        for (size_t i = 0; i < 12; i++)
            if (n[i])
                number_of_1[i]++;
            else
                number_of_0[i]++;
        free(n);
    }

    bool gammarr[12] = {0};

    for (size_t i = 0; i < 12; i++)
        if (number_of_1[i] > number_of_0[i])
            gammarr[i] = 1;

    unsigned short gamma = binary_to_number(gammarr, 0);
    unsigned short epsilon = ~gamma & 0b111111111111;
    
    printf("PART 1: %d\n", gamma * epsilon);

    rewind(fp);

    fp = bit_criteria(number_of_1[0] >= number_of_0[0], 1, fp);
    unsigned short o2 = binary_to_number(get_binary_from_file(fp), 1);

    fclose(fp);
    fp = fopen("input/day-3.input", "r");

    fp = bit_criteria(number_of_1[0] < number_of_0[0], 0, fp);
    unsigned short co2 = binary_to_number(get_binary_from_file(fp), 1);

    printf("PART 2: %d\n", o2 * co2);

    fclose(fp);
    return 0;
}