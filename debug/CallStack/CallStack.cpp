/*
 * 打印调用栈的方法android::CallStack,在C++代码中可用, 在C代码中存在链接错误, 暂时没有办法在C代码中使用
 * 使用方法,参考函数dump_stack()即可
 */
#include <stdio.h>
#include <utils/CallStack.h>

void dump_stack(void)
{
	android::CallStack cs(__func__);
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	dump_stack();
	return 0;
}
