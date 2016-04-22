#include <stdio.h>
#include <utils/CallStack.h>

void dump_stack(void)
{
	CallStack cs;
	cs.update();
	cs.dump();
}

int main(int argc, char **argv)
{
	dump_stack();

	return 0;
}