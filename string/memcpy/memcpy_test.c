#include <stdio.h>

#define memcpy_inner(dst, src, size) \
do { \
    typedef struct memory_t { unsigned char buf[size];} memory_T; \
    *(memory_T *)(dst) = *(memory_T *)(src); \
} while (0);

static int memcpy_unittest()
{
    int a[4] = {0x88996644, 0x69696969, 0x98889922, 0x01234455};
    int b[4] = {0x00};

    memcpy_inner(&b[0], &a[0], sizeof(a));
    printf("b[0] = 0x%x\n", b[0]);

    return 0;
}

int main()
{
    return memcpy_unittest();
}
