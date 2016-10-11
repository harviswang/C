#include <stdio.h>

static void funtion_pointer_type_test();
static void funtion_type_test();
int main(int argc, char *argv[])
{
    funtion_pointer_type_test();
    funtion_type_test();

	return 0;
}

typedef void (*func)(void); /* 声明 func_x 为函数指针类型 */

struct aoe {
    func f;
} xyz;

void test(void) { printf("test function pointer typer\n"); }

/*
 * 对函数指针取值(作*运算)结果和函数指针的值一样. 函数指针变量存放函数起始地址的变量
 */
static void funtion_pointer_type_test()
{
    void (*func_x)(void); /* 声明 func_x 为函数指针类型的变量 */
    func_x = test;
    xyz.f = test;
    printf("sizeof(func) = %ld\n", sizeof(func));

    func_x();
    (*func_x)();
    (********func_x)();
    xyz.f();   
}

/*
 * 函数类型的变量不能赋值(不能作为左值使用)
 * 函数类型的变量的sizeof值在gcc/clang为1, 在suncc为错误
 */
typedef void (func_t)(void); /* 声明 func_t为函数类型 */
static void funtion_type_test()
{
    func_t *func;
    func = test; /* pass */
    /* *func = test; */ /* failed */
    printf("sizeof(func) = %ld\n", sizeof(func));
}
