#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <string.h>

int main()
{
	char *str = "12,   0";
	char *pc = strchr(str, ',');
	int first, second;

	printf("*pc:%c\n", *pc);
	first = atoi(str);
	second = atoi(pc + 1);
	printf("first:%d\n", first);
	printf("second:%d\n", second);

	return 0;
}
