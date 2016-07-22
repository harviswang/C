#include <stdio.h>

/*
 * Conditional jump or move depends on uninitialised value(s)
 */

int main(int argc, char* argv[])
{
	int a[5];
	int i, s;

	a[0] = a[1] = a[3] = a[4] = 0; /* a[2] uninitialised */
	s = 0;

	for (i = 0; i < 5; i++) {
		s += a[i];
	}

	if (s == 377) {
		printf("sum is %d\n", s);
	}

	return 0;
}
