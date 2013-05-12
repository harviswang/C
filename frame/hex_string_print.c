#include "stdio.h"
#include "malloc.h"
//字节流变为对应的16进制字符流
#define m_malloc(x, y) (malloc((x)))
#define m_free(x) (free((x)))
static 
char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', 
              '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
void print_byte(char b)
{
	//前四位
	putchar(hex[(b & 0xF0) >> 4]); 
	//后四位 
	putchar(hex[b & 0x0F]); 
} 

void print_string(char *str, int len)
{
	int i;
	for (i = 0; i < len; i++)
		print_byte(str[i]);
}       
void print_hexstream(char *str, int len)
{
	char *cp = NULL, temp;
	int i, newlen;
	
	newlen = len*2;
	cp = (char *)m_malloc(newlen, 0);
	if (cp != NULL)
	{
		for (i = 0; i < len; i++)
		{
			temp = str[i];
			cp[i*2] = hex[(temp & 0xF0) >> 4];
			cp[i*2+1] = hex[temp & 0x0F];
		}
			
	}
//	uart_debug_print(cp, newlen);
	print_string(cp, newlen);
	m_free(cp);
}     

struct two_bytes
{
	char a;
	char b;
};
struct four_bytes
{
	char a;
	char b;
	char c;
	char d;
};

/*
main()
{
	char aoe[] = {0x12, 0x23, 0x45};
	char *charp;
	int i;
	int big = 0x89;
	print_string(aoe, ARRAY_SIZE(aoe));
	printf("\n");

	
	print_hexstream((char *)((struct four_bytes *)&big), 4);
	printf("\n");
	print_hexstream(aoe, 3);
	return 0;
}
*/
