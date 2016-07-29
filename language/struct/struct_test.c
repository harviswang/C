#include <stdio.h>
#include <string.h>
#include <assert.h>

static void struct_zero_init_test();
int main(int argc, char *argv[])
{
    printf("struct test ...\n");
    struct_zero_init_test();
    
    return 0;
}

/*
 * 结构体通过{0}赋值, 其成员各个成员值均为0
 * struct aoe abc = { 0 }; 和下面的表达式等价
 * struct aoe abc;
 * memset(&abc, 0, sizeof(abc));
 */
static void struct_zero_init_test()
{
    struct aoe {
        int a;
        int o;
        int e;
        char buf[10];
    };
    
    struct aoe abc = { 0 };
    int i;

    assert(abc.a == 0);
    assert(abc.o == 0);
    assert(abc.e == 0);
    for (i = 0; i < 10; i++) {
        assert(abc.buf[i] == 0);
    }
#if defined(DEBUG)
    printf("abc.a = %d\n", abc.a);
    printf("abc.o = %d\n", abc.o);
    printf("abc.e = %d\n", abc.e);
    for (i = 0; i < 10; i++) {
        printf("%c\n", abc.buf[i]);
    }
#endif
}