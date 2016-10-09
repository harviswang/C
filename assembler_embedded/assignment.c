#include <stdio.h>

static void assignment_test(void);

int main(int argc, char *argv[])
{
	assignment_test();
	return 0;
}

/*
 * "b" is the output operand, referred to by %0 and "a" is the input operand, referred to by %1. 
 * "r" is a constraint on the operands. "r" says to GCC to use any register for storing the operands.
 * "=" says that it is the output operand and is write-only.
 * "%%" There are two %’s prefixed to the register name. This helps GCC to distinguish between the operands and registers. operands have a single % as prefix. 
 * The clobbered register %eax after the third colon tells GCC that the value of %eax is to be modified inside "asm", so GCC won’t use this register to store any other value.
 */
static void assignment_test(void)
{
	int a = 10;
	int b;
#if defined(__x86_64__)
    	asm ("mov %1, %%eax \n" /* %1 -> %%eax */
	         "mov %%eax, %0 \n" /* %%eax -> %0 */
            : "=r"(b)  /* output */
            : "r"(a)   /* input */
            : "%eax"   /* clobbered register */
	);
#elif defined(__mips__)
    	asm ("move $8, %1 \n" /* %1 -> $8 */
	         "move %0,  $8 \n" /* $8 -> %0 */
            : "=r"(b)  /* output */
            : "r"(a)   /* input */
            : "$8"    /* clobbered register */
	);        
#endif    


	if (a == b) {
		printf("b = %d\n", b);
	} else {
        printf("assemble code with error! a = %d b = %d\n", a, b);
    }
}
