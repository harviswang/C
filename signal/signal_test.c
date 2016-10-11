#include <stdio.h>
#include <sys/signal.h>

static void sigtrap_test(void);

int main(int argc, char *argv[])
{
	sigtrap_test();
	return 0;
}

/*
 * 参数x这里是SIGTRAP的值, 是信号的值
 */
void sigtrap_handler(int x)
{
    printf("%s() %d x = %d\n", __func__, __LINE__, x);
}

static void sigtrap_test(void)
{
    signal(SIGTRAP, sigtrap_handler);
#if defined(__x86_64__)
    asm ("int $0x3\n");
#elif defined(__mips__)     
#define BRK_BUG 512
	asm("break %0" : : "i" (BRK_BUG));
#endif    
}
