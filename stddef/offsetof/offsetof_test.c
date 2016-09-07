#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    struct s {
        int i;
        char c;
        double d;
        char a[];
    };

    printf("offsets: i=%ld; c=%ld; d=%ld a=%ld\n",
            offsetof(struct s, i),
            offsetof(struct s, c),
            offsetof(struct s, d),
            offsetof(struct s, a));
    printf("sizeof(struct s)=%ld\n", sizeof(struct s));

    struct s_packed {
        int i;
        char c;
        double d;
        char a[];
    } __attribute__((packed));

    printf("offsets: i=%ld; c=%ld; d=%ld a=%ld\n",
            offsetof(struct s_packed, i),
            offsetof(struct s_packed, c),
            offsetof(struct s_packed, d),
            offsetof(struct s_packed, a));
    printf("sizeof(struct s)=%ld\n", sizeof(struct s_packed));

    exit(EXIT_SUCCESS);
}

