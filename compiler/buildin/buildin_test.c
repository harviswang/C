#include <stdio.h>

static void __builtin_expect_test(void);
static void __builtin_constant_p_test(void);
int main(int argc, char **argv)
{
    __builtin_expect_test();
    __builtin_constant_p_test();

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
}
