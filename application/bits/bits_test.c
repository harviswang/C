#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
 * a.out 0x0000040a 0 3 => 0xa
 * a.out 0x0000040a 0 11 => 0x40a
 * a.out 0x0000040a 3 => 0x1
 */
int main(int argc, char *argv[])
{
    long long n, result;
    unsigned char bit_from, bit_to;

    if (argc < 2 || argc > 4) {
        printf("usage: %s hex_number bit_from [bit_to]\n", argv[0]);
        exit(-1);
    }

    n = strtoll(argv[1], (char **)NULL, 16);
    bit_from = (unsigned char)strtol(argv[2], (char **)NULL, 10);
    if (argc == 4) {
        bit_to = (unsigned char)strtol(argv[3], (char **)NULL, 10);
        if (bit_from > bit_to) {
            unsigned char tmp = bit_from;
            bit_from = bit_to;
            bit_to = tmp;
        }
    } else {
        bit_to = bit_from;
    }

    result = (n >> bit_from) & ((1LL << (bit_to - bit_from + 1)) - 1);
    printf("%s[%d, %d] = 0x%llx, in decimal it's %lld\n", argv[1], bit_from, bit_to, result, result);

    return(0);
}
