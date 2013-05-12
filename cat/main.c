#include <stdio.h>
#include <stdlib.h>
#include "djyos_transplant.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
typedef unsigned int u32;
typedef unsigned char u8;
int main(int argc, char *argv[]) {
	
	djyfs_file *fp;
	u32 n;
	u8 buf;
	
	fp = djyfs_fopen("./cat.c", "rb");
	
	while (n = fread((void *)&buf, 1, 1, fp))
	{
		printf("%c", buf);
	}
	djyfs_fclose(fp);
	return 0;
}
