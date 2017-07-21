#include <stdio.h>

int main(int argc, char *argv[])
{
	union byteorder_ {
		short s;
		char c[sizeof(short)];
	} byteorder;

	byteorder.s = 0x0102;
	int i;
	for (i = 0; i < sizeof(short); i++) {
		printf("byteorder.c[%d]=0x%02x\n", i, byteorder.c[i]);
	}
	if ( byteorder.c[0] == 0x01) {
		printf("big-endian\n");
	} else if (byteorder.c[0] == 0x02) {
		printf("little-endian\n");
	} else {
		printf("unknown-endian\n");
	}

	return 0;
}
