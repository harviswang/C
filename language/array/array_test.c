#include <stdio.h>
#include <assert.h>

static void function_as_index(void);
static void array_name_test();

int main(int argc, char **argv)
{
    function_as_index();
    array_name_test();
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
