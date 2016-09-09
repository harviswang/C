#include <stdio.h>
#include <assert.h>

static void function_as_index(void);
static void array_name_test();
static void array_size_test();

int main(int argc, char **argv)
{
    function_as_index();
    array_name_test();
    array_size_test();

	return 0;
}

static int get_index(void)
{
    return 2;
}
/*
 * 数组的索引,可以使用函数调用获取
 */
static void function_as_index(void)
{
    int array[] = {1, 2, 3, 4};
    printf("%d\n", array[get_index()]);
}

/*
 * 数组名的值和数组名取地址运算的值, 在值上面是相等的.
 * 示例中week是一个字符常量指针, &week是一个数组指针, 类型不同
 */
static void array_name_test()
{
    char week[7];
    printf("week  = %p\n", week);
    printf("&week = %p\n", &week);
    assert(week == (char *)&week);
    printf("week  + 1 = %p\n", week + 1);
    printf("&week + 1 = %p\n", &week + 1);
}

/*
 * 宏ARRAY_SIZE
 * 数组的初始化也可以制定索引号, 进行逐个初始化
 */
static void array_size_test()
{
#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))
    const char *week[7] = {
        [0] = "sunday",
        [1] = "monday",
        [2] = "tuesday",
        [3] = "wednesday",
        [4] = "thursday",
        [5] = "friday",
        [6] = "saturday",
    };
    printf("ARRAY_SIZE(week) = %ld\n", ARRAY_SIZE(week));
}
