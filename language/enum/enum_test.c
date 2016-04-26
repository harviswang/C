#include <stdio.h>

typedef void (*function_t)(void);

void enum_typedef_test(void);
static function_t enum_test[] = {
		enum_typedef_test,
};

int main(int arc, char **argv)
{
	int i;
	
	for (i = 0; i < sizeof(enum_test)/sizeof(enum_test[0]); i++) {
		enum_test[i]();
	}

	return 0;
}

/*
 * enum_me_t can only be used in the block of function enum_typedef_test()
 * A and C can be equal
 */
void enum_typedef_test(void)
{
	/* printf("A = %d\n", A); */
	typedef enum /* name_can_ignore_here*/ {
		A = 0,
		B = 1,
		C = A,
	} enum_me_t;
	
	enum_me_t a = B;
	printf("enum_me_t a = %d\n", a);
	
	if (C == A) {
		printf("two element in an enum can be equal\n");
	} else {
		printf("two element in an enum can NOT be equal\n");
	}
}