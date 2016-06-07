#include <stdio.h>
#include <stdlib.h>

/*
 * assert macro
 */
#define LOG_ASSERT(condition, format...) \
    do {if (!(condition)) {printf("Assert failed: " #condition ": "format);}}while(0);

static void variable_argument_macro_function_test();
static void gcc_version_dump_test();
static void var_name_unchange_test();
static void function_name_ifdef_test();
int main(int argc, char **argv)
{

    var_name_unchange_test();
	function_name_ifdef_test();
	gcc_version_dump_test();
	variable_argument_macro_function_test();
	return 0;
}

/*
 * 宏参数原样使用, 不做任何解析
 */
static void var_name_unchange_test()
{
    /* 宏作用域只有上下位置之分, 没有{}块内外之分 */
    {
        /*
         * 打印变量名(不对变量求值，实际是将变量名当作字符串)和行号
         * 主要使用宏语法里面的关键字'#'
         */
        #define MY_MUTEX_LOC(_lock) do {         \
            printf("%s %d\n", #_lock, __LINE__); \
            printf("unlock .....\n");            \
        }while(0)
    }
    int a = 88;
    (void)a; /* avoid annoying warning*/
    MY_MUTEX_LOC(&a); /* ‘&a’ 被当作一个整体打印出来 */
}

static void barrier(){}
/*
 * 测试函数名是否可以被#ifdef识别
 * *不会被识别*
 */
static void function_name_ifdef_test()
{
#ifdef barrier
    printf("%s %s():%d\n", __FILE__, __func__, __LINE__);
#endif
    barrier();
}

/*
 * works both gcc and clang in centos 7
 */
static void gcc_version_dump_test()
{
    /*
     * Common definitions for all gcc versions go here.
     */
    #define GCC_VERSION (__GNUC__ * 10000 \
               + __GNUC_MINOR__ * 100 \
               + __GNUC_PATCHLEVEL__)

    printf("GCC_VERSION is %d\n", GCC_VERSION);
    printf("__GNUC__ is %d\n", __GNUC__);
    printf("__GNUC_MINOR__ is %d\n", __GNUC_MINOR__);
    printf("__GNUC_PATCHLEVEL__ is %d\n", __GNUC_PATCHLEVEL__);
}

static void variable_argument_macro_function_test()
{
    int a = 8;
    LOG_ASSERT(a > 9, "%d is not bigger than 9\n", a);
}
