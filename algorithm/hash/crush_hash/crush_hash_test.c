#include "hash.h"
#include <stdio.h>

int main()
{
	__u32 keys[]  = {0x00, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0xFFFFFFFF};
	__u32 hash_v;
	int i;

	for (i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
		hash_v = crush_hash32(CRUSH_HASH_RJENKINS1, keys[i]);
		printf("crush_hash(0x%x) = 0x%x\n", keys[i], hash_v);
	}

	return 0;
}
