#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

unsigned long parse_packet(FILE *fp);


unsigned char result[256];
unsigned char get_nibble(FILE *fp) {
    unsigned char c = fgetc(fp);
    return result[c];
}

FILE *decode_hexa_file(FILE *fp) {
    FILE *ret = tmpfile();

    unsigned char nibble;
    while ((nibble = get_nibble(fp)) < (1 << 4)) {
        char bits[4];
        for (size_t i = 0; i < 4; i++)
            bits[i] = !!((nibble << i) & 1 << 3);
        fwrite(bits, sizeof(*bits), 4, ret);
    }
    fclose(fp);
    rewind(ret);
    return ret;
}

typedef unsigned char version_t;
version_t get_packet_version(FILE *fp) {
    char p[3];
    fread(p, sizeof(*p), 3, fp);
    return p[0] * 4 + p[1] * 2 + p[2];
}

typedef unsigned char typeid_t;
typeid_t get_packet_typeid(FILE *fp) {
    char p[3];
    fread(p, sizeof(*p), 3, fp);
    return p[0] * 4 + p[1] * 2 + p[2];
}

unsigned long parse_literal_number(FILE *fp) {
    unsigned long ret = 0;

    bool continue_ = 1;
    while (continue_) {
        continue_ = fgetc(fp);
        for (size_t i = 0; i < 4; i++) {
            ret <<= 1;
            ret += fgetc(fp);
        }
    }
    return ret;
}

typedef unsigned char length_type_id_t;
length_type_id_t get_length_type_id(FILE *fp) {
    return fgetc(fp);
}

typedef unsigned short length_t;
length_t get_length(FILE *fp, size_t size) {
    length_t ret = 0;
    for (size_t i = 0; i < size; i++) {
        ret <<= 1;
        ret += fgetc(fp);
    }
    return ret;
}

void result_op(unsigned long *result, typeid_t id, unsigned long num) {
    if (id == 0)
        *result += num;
    else if (id == 1)
        *result *= num;
    else if (id == 2) {
        if (num < *result)
            *result = num;
    } else if (id == 3) {
        if (num > *result)
            *result = num;
    }
}

unsigned long parse_operator_packet(FILE *fp, typeid_t id) {
    length_type_id_t lt = get_length_type_id(fp);
    length_t l = get_length(fp, lt ? 11 : 15);
    if (id >= 5 && id <= 7) {
        unsigned long a = parse_packet(fp);
        unsigned long b = parse_packet(fp);
        return id == 5 ? a > b
             : id == 6 ? a < b
             : id == 7 ? a == b : 0;
    }
    unsigned long result = id == 0 ? 0
                        : id == 1 ? 1
                        : id == 2 ? 0 - 1
                        : id == 3 ? 0 : 0;
    if (!lt) {
        // LENGTH TYPE 0
        long pos = ftell(fp);
        while (ftell(fp) < pos + l)
            result_op(&result, id, parse_packet(fp));
    } else {
        // LENGTH TYPE 1
        for (size_t i = 0; i < l; i++)
            result_op(&result, id, parse_packet(fp));
    }
    return result;
}

unsigned int version_sum = 0;
unsigned long parse_packet(FILE *fp) {
    version_t version = get_packet_version(fp);
    version_sum += version;
    typeid_t id = get_packet_typeid(fp);

    if (id == 4)
        return parse_literal_number(fp);
    return parse_operator_packet(fp, id);
}

int main() {
    for (size_t i = 0; i < 256; i++)
        result[i] = 1 << 4;
    for (size_t i = 48; i <= 57; i++)
        result[i] = i - 48;
    for (size_t i = 65; i <= 70; i++)
        result[i] = i - 55;



    
    FILE *fp = decode_hexa_file(fopen("input/day-16.input", "r"));


    unsigned long result = parse_packet(fp);
    printf("PART 1: %u\n", version_sum);
    printf("PART 2: %lu\n", result);

    fclose(fp);
    return 0;
}