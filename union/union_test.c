#include <stdio.h>
static void union_sizeof_test(void);
int main(int argc, char **argv)
{
    printf("sizeof(struct efi_header) = %ld\n", sizeof(struct efi_header));
    union_sizeof_test();
    return 0;
}

/*
 * sizeof(union var)是var中最大的成员的sizeof值
 */
static void union_sizeof_test(void)
{
    struct efi_header{
        unsigned char magic;
        unsigned char version;
    };
    
    union {
        struct efi_header header;
        unsigned char block[512];
    }un;
    
    printf("sizeof(un.header) = %ld\n", sizeof(un.header));
    printf("sizeof(un.block) = %ld\n", sizeof(un.block));
    printf("sizeof(un) = %ld\n", sizeof(un));
    if (sizeof(un.header) > sizeof(un.block)) {
        if (sizeof(un) == sizeof(un.header)) {
            printf("sizeof(un) == sizeof(un.header)\n");
        } else {
            printf("sizeof(un) != sizeof(un.header), why?\n");
        }
    } else {
        if (sizeof(un) == sizeof(un.block)) {
            printf("sizeof(un) == sizeof(un.block)\n");
        } else {
            printf("sizeof(un) != sizeof(un.block), why?\n");
        }
    }

    /*
     * 修改un.block可以修改un.header
     */
    un.block[0] = 'U';
    un.block[1] = 'N';
    un.block[2] = 'I';
    un.block[3] = 'O';
    un.block[4] = 'N';
    
    printf("un.header.magic = %c\n", un.header.magic);
    printf("un.header.version = %c\n", un.header.version);

    /*
     * 修改un.header可以修改un.block
     */
    unsigned char *p = &un.header.version;
    p[1] = 'X';
    p[2] = 'Y';
    p[3] = 'Z';
    printf("un.block[2] = %c\n", un.block[2]);
    printf("un.block[3] = %c\n", un.block[3]);
    printf("un.block[4] = %c\n", un.block[4]);
}