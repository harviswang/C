/*
 * 应用程序中打印call stack调用栈的方法
 * 需要编译选项'-rdynamic'的支持
 */
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>


void dump_stack()
{
#define SIZE 100
	void *buffer[SIZE]; /* 各个元素存储的是 栈帧 的地址, 并没有进一步的malloc空间 */
	char **strings;
	int n;
	int i;
	
	int err = 0;
	
	n = backtrace((void **)buffer, SIZE);
	if (n == 0) {
		printf("Empty calling stack\n");
		return ;
	}

	strings = backtrace_symbols((void *const *)buffer, n);
	if (strings == NULL) {
		printf("err = %d\n", err);
		return ;
	}

	for (i = 0; i < n; i++)
		printf("%s\n", strings[i]);

	free(strings);
#undef SIZE
}

static /* "static" means don't export the symbol... 就是dump_stack()无法打印出它的名字 */
void test()
{
	dump_stack();
}

int main(int argc, char **argv)
{
	test();
	return 0;
}