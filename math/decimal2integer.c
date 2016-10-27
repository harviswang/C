#include <stdio.h>
/*  in 32bits MAX(unsigned long) = 4294967295 */
static
unsigned long tens[] = { 1, 
                         10, 
                         100, 
                         1000, 
                         10000, 
                         100000, 
                         1000000, 
                         10000000,
                         100000000,
                         1000000000,};
/*
 * 小数部分是10的多少次方, 次方的结果存储在*flags中
 * 返回10的*flags次方的值
 */
static unsigned long toten(unsigned long fraction, unsigned long *flags)
{
	unsigned long i = 0;
	for ( ; i < sizeof(tens) / sizeof(tens[0]) && tens[i] < fraction; i++) {
	}
	
	if (i == sizeof(tens) / sizeof(tens[0])) {
		return -1;
	} else {
        if (flags) *flags = i;
		return tens[i];
    }
}

/*
 * 将一个由宏DECIMAL()产生的数, 分离出小数部分(*fraction_part存储)和整数部分(*integer_part存储)
 * fraction是函数toten()中*flags的值, 是小数点的位数
 */
unsigned long todecimal(unsigned long integer, unsigned long fraction, 
                  unsigned long *integer_part, unsigned long *fraction_part)
{
    if (fraction + 1 > sizeof(tens) / sizeof(tens[0])) {
        return -1;
    } else {
        *integer_part = integer / tens[fraction];
        *fraction_part = integer - *integer_part * tens[fraction];
        return 0;
    }
}

#define DECIMAL(integer, fractional, flags) (integer * toten(fractional, flags) + (fractional))
int main(int argc, char *argv[])
{
    unsigned long integer, fraction, flags;
	printf("%lu\n", DECIMAL(1, 8375, &flags));
    printf("flags = %ld\n", flags);
    todecimal(DECIMAL(1, 8375, &flags), flags, &integer, &fraction);
    printf("integer = %ld\n", integer);
    printf("fraction = %ld\n", fraction);
	return 0;
}

