#include <stdio.h>
#include "unwind_backtrace.h"
#include <cutils/log.h> /* ALOGI */

void dump_stack(void)
{
	getCallStack();
	ALOGI("-----------------");
	do_backtrace();
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    ALOGI("harvis1");
    dump_stack();
    ALOGI("harvis2");
    return 0;
}
