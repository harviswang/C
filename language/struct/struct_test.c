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
 * 结构体通过{}赋值, 其成员各个成员值均为0, 仅仅针对无嵌套struct
 * gcc/clang可行, suncc报错(syntax error:  empty initializer)
 * 还是不用{}, 需要用的时候, 选择{0}或者{{0}}这种形式
 * 针对有嵌套的struct,需要类似{{0}, }
 * struct aoe abc = {}; 和下面的表达式等价
 * struct aoe abc;
 * memset(&abc, 0, sizeof(abc));
 */
static void struct_zero_init_test()
{
    struct block {
        int a;
        char b;
        int c;

    };

    struct block abc = {0};
    struct block xyz;

    printf("sizeof(struct block) = %ld\n", sizeof(struct block));

    /* test case 1 */
    memset(&xyz, 0, sizeof(xyz));
    if (!memcmp(&abc, &xyz, sizeof(struct block))) {
        printf("[struct block abc = {};] == [struct block xyz; memset(&xyz, 0, sizeof(xyz));]\n");
    }
}

