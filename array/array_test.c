#include <stdio.h>

static void function_as_index(void);

int main(int argc, char **argv)
{

    function_as_index();
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
