#include <stdio.h>

/*
 * 函数参数和返回值都是int类型的
 */
int foo(int x)
{
	return 2 + x;
}

/*
 * 函数参数和返回值都是const int类型的
 */
const int bar(const int x)
{
	return 2 * x;
}

/*
 * 测试用于验证C函数的返回值并不像C++那样返回的对象都是const类型的
 * C函数的返回值没有特殊
 */
int main(int argc, char **argv)
{
	int a = 8;
	
	const int b = bar(foo(a));
	const int c = foo(bar(a));
	printf("b = %d\n", b);
	printf("c = %d\n", c);


	printf("%d\n", bar(a));
	printf("%d\n", foo(b));
	return 0;
}
