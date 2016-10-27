/*
 * basic Asm format:
 * asm("assembly code");
 *
 * extended Asm format:
 * asm ( assembler template
 *     : output operands             // optional
 *     : input operands              // optional
 *     : list of clobbered registers // optional
 *     );
 *
 * constraint on the operand:
 * "r": Register operand constraint(r),
 *      says to GCC to use any register for storing the operand.
 * +---+--------------------+
 * | r | Register(s)        |
 * +---+--------------------+
 * | a | %eax, %ax, %al     |
 * | b | %ebx, %bx, %bl     |
 * | c | %ecx, %cx, %cl     |
 * | d | %edx, %dx, %dl     |
 * | S | %esi, %si          |
 * | D | %edi, %di          |
 * +---+--------------------+
 * "=": output operand constraint should have a constraint "="
 *      his modifier says that it is the output operand and is write-only
 * "0":
 * "c":
 * "g": Any register, memory or immediate integer operand is allowed, except
        for registers that are not general registers.
 * "m": Memory operand constraint(m), Memory constraints can be used most
 *      efficiently in cases where a C variable needs to be updated inside "asm"
 *
 * clobbered constraint:
 * "memory": This will cause GCC to not keep memory values cached in registers
 *           across the assembler instruction.
 * "cc":
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>

static void assignment_test(void);
static void set_bit_test(void);
static void div_test(void);
static int movl_test(void);
static void five_times_x_test(void);
static void call_test(void);
static void m_test(void);

int main(int argc, char *argv[])
{
    div_test();
    set_bit_test();
	assignment_test();
    printf("eax = 0x%x\n", movl_test());
    five_times_x_test();
    call_test();
    m_test();

	return 0;
}

/*
 * 参数x这里是SIGTRAP的值, 是信号的值
 */
#if defined(__mips__)
void signal_trap_handler(int x, struct siginfo *y, void *z)
{
    (void)y;
    (void)z;
    printf("%s() %d x = %d\n", __func__, __LINE__, x);
    exit(0);
}
#else
void signal_trap_handler(int x)
{
    printf("%s() %d x = %d\n", __func__, __LINE__, x);
}
#endif

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
		asm ("movl %1, %0 \n" /* == %1 -> %%eax, %%eax -> %0 */
            : "=r"(b)  /* output */
            : "r"(a)   /* input */
            : "eax"   /* clobbered register, can't use %%eax here. use eax ok */
	);
#elif defined(__mips__)
        struct sigaction action;
        memset(&action, 0, sizeof(action));
        action.sa_flags = SA_SIGINFO;
        action.sa_sigaction = signal_trap_handler;
        sigaction(SIGTRAP, &action, NULL);
		asm ("move %0, %1 \n" /* == %1 -> $8, $8 -> %0 */
            : "=r"(b)  /* output */
            : "r"(a)   /* input */
            : "$8"    /* clobbered register */
	);        
#define BRK_BUG 512
	__asm__ __volatile__("break %0" : : "i" (BRK_BUG));
#endif    

	if (a == b) {
		printf("b = %d\n", b);
	} else {
        printf("assemble code with error! a = %d b = %d\n", a, b);
    }
}

#define __LL		"ll	"
#define __SC		"sc	"
#define __INS		"ins    "
#define __EXT		"ext    "
#define SZLONG_LOG 5
#define SZLONG_MASK 31UL
static inline void set_bit(unsigned long nr, volatile unsigned long *addr)
{
#if defined(__mips__)
	unsigned long *m = ((unsigned long *) addr) + (nr >> SZLONG_LOG);
	unsigned short bit = nr & SZLONG_MASK;
	unsigned long temp;

		do {
			__asm__ __volatile__(
			"	" __LL "%0, %1		# set_bit	\n"
			"	" __INS "%0, %3, %2, 1			\n"
			"	" __SC "%0, %1				\n"
			: "=&r" (temp), "+m" (*m)
			: "ir" (bit), "r" (~0));
		} while (!temp);
#else
	asm("bts %1,%0" : "+m" (*(unsigned long *)addr) : "Ir" (nr));
#endif
}


static void set_bit_test()
{
    unsigned long xx = 0x00;
    set_bit(0, &xx);
    set_bit(1, &xx);
    printf("xx = 0x%lx\n", xx);
}

static void div_test()
{
#if defined(__mips__)
    int a = 10;
    int b = 4;
    int c;
    asm("div  %0, %1, %2\n"
        : "=r"(c)          /* output */
        : "r"(a), "r"(b)   /* input */
    );

    printf("c = %d\n", c);
#endif
}

static int movl_test()
{
    int x = 0;
#if defined(__x86_64__)
    asm("movl %%ecx, %%eax\n" /* moves the contents of ecx to eax */
        "movl %%eax, %0\n"
        : "=r" (x)/* output */
        : /* input */
        :
    );
#endif
    return x;
}

static void five_times_x_test()
{
    int x = 8;
    int five_times_x = 0;
    asm ("leal (%1, %1, 4), %0\n"
         : "=r" (five_times_x)
         : "r" (x));
    printf("five_times_x = %d\n", five_times_x);
}

static void _foo(int from, int to)
{
    printf("from -> to = %d -> %d\n", from, to);
}

static void call_test()
{
    int from = 2;
    int to = 8;

    (void)_foo;
    asm ("mov %0, %%rdi\n"
         "mov %1, %%rsi\n"
         "call _foo\n"
         : /* no outputs */
         : "g" (from), "g" (to)
         : "rdi", "rsi"
    );
}

static void m_test()
{
    int i;
    struct sidt{
        unsigned char byte[10];
    } loc = {{0, } };

    asm("sidt %0\n" : : "m" (loc));
    printf("loc = 0x");
    for(i = 9; i >= 0; i--) {
        printf("%x", loc.byte[i]);
    }
    printf("\n");
}