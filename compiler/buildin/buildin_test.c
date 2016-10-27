#include <stdio.h>

static void __builtin_expect_test(void);
static void __builtin_constant_p_test(void);
static void __builtin_popcount_test(void);
static void __builtin_unreachable_test(void);
static void __builtin_return_address_test(void);

int main(int argc, char **argv)
{
    __builtin_expect_test();
    __builtin_constant_p_test();
    __builtin_popcount_test();
    __builtin_unreachable_test();
    __builtin_return_address_test();

    return 0;
}

/*
 * likely: in cases when the likeliest branch is very very very likely,
 * unlikely: in cases when the unlikeliest branch is very very very unlikely.
 * 仔细看看反汇编的指令 TODO
 */
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
static void __builtin_expect_test(void)
{
    int a = 3;

    if (likely (a == 2)) {
        a++;
    } else {
        a--;
    }

    printf("a = %d\n", a);
}

/*
 * __builtin_constant_p 是编译器gcc内置函数，用于判断一个值是否为编译时常量，如果是常数，函数返回1 ，否则返回0。
 * 此内置函数的典型用法是在宏中用于手动编译时优化。
 * __builtin_constant_p受编译优化选项影响,
 * case 3/5 在gcc -O2时候ok, 在gcc -O0是failed.
 * case 5 在clang -O2 failed
 */
static inline void help(int a, int b)
{
    if (__builtin_constant_p(a) && __builtin_constant_p(b)) {
        printf("__builtin_constant_p argument is ok\n");
    }
}
static void __builtin_constant_p_test(void)
{
    int ret;
    const int sci = 999;
    const long *pcl = 0;
#define CONSTANT 222
    /* case 1: 宏常数 */
    if ((ret = __builtin_constant_p(CONSTANT))) {
        printf("__builtin_constant_p(CONSTANT) = %d\n", ret);
    }

    /* case 2: 字符串常量 */
    if ((ret = __builtin_constant_p("string"))) {
        printf("__builtin_constant_p(\"string\") = %d\n", ret);
    }

    /* case 3: 常数 const int */
    if ((ret = __builtin_constant_p(sci))) {
        printf("__builtin_constant_p(sci) = %d\n", ret);
    }

    /* case 4: 指针常数, 运行时决定, __builtin_constant_p()返回0 */
    if ((ret = __builtin_constant_p(pcl))) {
        printf("__builtin_constant_p(pcl) = %d\n", ret);
    }

    /* case 5: 宏调用, 会返回1 */
#define HELP(a, b) help((a), (b))
    HELP(0x23, 0x45);

    /* case 6: NULL */
    if ((ret = __builtin_constant_p(NULL))) {
        printf("__builtin_constant_p(NULL) = %d\n", ret);
    }

    /* case 7: !variable */
    if ((ret = __builtin_constant_p(!ret))) {
        printf("__builtin_constant_p(!ret) = %d\n", ret);
    }

    /* case 7: !!variable */
    if ((ret = __builtin_constant_p(!!ret))) {
        printf("__builtin_constant_p(!!ret) = %d\n", ret);
    }

    /* case 8: variable & 0x03 */
    if ((ret = __builtin_constant_p(ret & 0x03))) {
        printf("__builtin_constant_p(ret & 0x03) = %d\n", ret);
    }
}

/*
 * __builtin_popcount计算二进制数(32位, 类型是unsigned type)中1的个数
 */
static void __builtin_popcount_test(void)
{
    printf("%s() test..\n", __func__);
    printf("__builtin_popcount(4)=%d\n", __builtin_popcount(4));
    printf("__builtin_popcount(7)=%d\n", __builtin_popcount(7));
    //printf("__builtin_popcount(0x1ffffffff)=%d\n", __builtin_popcount(0x1ffffffff));
}

/*
 * __builtin_unreachable() 在应用中调用会导致Segmentation fault (core dumped)
 * 在linux kernel中调用会导致重启
 */
static void __builtin_unreachable_test(void)
{
   // __builtin_unreachable();
}

/*
 * __builtin_return_address(0) return type is void*
 */
#define return_address() ({__builtin_return_address(0);})
static void __builtin_return_address_test()
{
    printf("%s return address :%p\n", __func__, return_address());
}