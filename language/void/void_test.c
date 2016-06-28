#include <stdio.h>

/*
 * 编译时使用-Werror=strict-prototypes时, gcc要求函数声明括号里面要有void关键字
 * 函数实现括号里面可以没有void关键字, clang没有此约束
 */
static void void_as_argument();
int main(int argc, char **argv)
{
	printf("void test...\n");
	void_as_argument();

	return 0;
}

static void void_as_argument()
{
    printf("%s\n", __func__);
}
