#include <stdio.h>
#include <stdlib.h>

/*
 * 打印变量名(不对变量求值，实际是将变量名当作字符串)和行号
 * 主要使用宏语法里面的关键字'#'
 */
#define	MY_MUTEX_LOC(_lock) do { \
printf("%s %d\n", #_lock, __LINE__);	\
printf("unlock ....."); \
}while(0)

int main(void)
{
	int a = 88;
	MY_MUTEX_LOC(&a); /* ‘&a’ 被当作一个整体打印出来 */
}
