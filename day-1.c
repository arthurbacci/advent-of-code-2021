#include <stdio.h>
#include <stdlib.h>

int get_number_from_file(FILE *fp) {
    char *buf = NULL;
    size_t bufcap = 0;
    size_t len = getline(&buf, &bufcap, fp);
    
    if (len != -1 && buf[len - 1] == '\n')
        buf[--len] = '\0';
    
    if (len == -1 || len == 0) {
        free(buf);
        return -1;
    }

    int number = atoi(buf);
    free(buf);
    return number;
}

unsigned int number_of_increases(FILE *fp) {
    int lastnumber = -1;
    unsigned int ret = 0;
    while (1) {
        int number = get_number_from_file(fp);
        if (number == -1)
            break;
        if (number > lastnumber && lastnumber >= 0)
            ret++;
        lastnumber = number;
    }
    return ret;
}

int main() {
    FILE *fp = fopen("input/day-1.input", "r");

    printf("PART 1: %d\n", number_of_increases(fp));

    rewind(fp);


    FILE *tmp = tmpfile();


    int number1 = get_number_from_file(fp);
    int number2 = get_number_from_file(fp);
    int number3 = get_number_from_file(fp);

    while (1) {
        fprintf(tmp, "%d\n", number1 + number2 + number3);
        number1 = number2;
        number2 = number3;
        number3 = get_number_from_file(fp);
        if (number3 == -1)
            break;
    }

    fclose(fp);

    rewind(tmp);

    printf("PART 2: %d\n", number_of_increases(tmp));

    fclose(tmp);
    return 0;
}
