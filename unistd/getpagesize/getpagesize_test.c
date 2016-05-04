#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	printf("getpagesize() = 0x%08x\n", getpagesize());
#ifdef __ANDROID__
	printf("PAGE_SIZE = 0x%08x\n", PAGE_SIZE);
	printf("PAGE_MASK = 0x%08x\n", PAGE_MASK);
	printf("PAGE_SHIFT = 0x%08x\n", PAGE_SHIFT);
#endif
	return 0;
}
