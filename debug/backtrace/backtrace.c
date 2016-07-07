/*
 * 应用程序中打印call stack调用栈的方法
 * 需要编译选项'-rdynamic'的支持
 */
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  /* open() */
#include <unistd.h> /* close() */
#include <string.h>
#include <errno.h> /* errno */

void dump_stack()
{
    const size_t SIZE = 32;
	void *buffer[SIZE]; /* 各个元素存储的是 栈帧 的地址, 并没有进一步的malloc空间 */
	char **strings;
	int n;
	int i;
	
	int err = 0;
	/*
	 * backtrace指向的是stack frame, 打印出来不可读
	 */
	n = backtrace((void **)buffer, SIZE);
	if (n == 0) {
		printf("Empty calling stack\n");
		return ;
	}
	/* 打印出来不是可读的字符 */
	for (i = 0; i < n; i++) {
	    printf("%s\n", (char *)buffer[i]);
	}

	/*
	 * 将不可读的buffer转换为可读的symbol数组
	 */
	strings = backtrace_symbols((void *const *)buffer, n);
	if (strings == NULL) {
		printf("err = %d\n", err);
		return ;
	}

	for (i = 0; i < n; i++) {
		printf("%s\n", strings[i]);
	}

	free(strings);

	/*
	 * 将调用栈信息打印到文件中
	 */
	char path[] = "/dev/shm/XXXXXX";
	char *file_name = mktemp(path);
	printf("file_name:%s\n", file_name);
	if (strcmp(file_name, "")) {
        int fd = open(file_name, O_RDWR | O_CREAT, S_IRWXU);
        if (fd != -1) {
            backtrace_symbols_fd((void *const *)buffer, n, fd);
            close(fd);
        } else {
            printf("open() failed, Error:%s\n", strerror(errno));
        }
	}
}

static /* "static" means don't export the symbol... 就是dump_stack()无法打印出它的名字 */
void test()
{
	dump_stack();
}

void test2()
{
    test();
}

void test3()
{
    test2();
}

int main(int argc, char **argv)
{
	test3();
	return 0;
}
