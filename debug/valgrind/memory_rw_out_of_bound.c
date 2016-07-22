#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    const int len = 4;
    int* pt = (int *)malloc(len * sizeof(int));
    if (pt) {
        int* p = pt;
        int i;
        
        /*
         * i   p
         * 0   p[1]
         * 1   p[2]
         * 2   p[3]
         * 3   p[4]
         */
        for (i = 0; i < len; i++) {
            p++;
        }
        *p = 5; /* pt[4] = 5; 写越界 */
        printf("the value of p equal: %d\n", *p); /* 读越界 */
        
        free(pt);
        pt = NULL;
    }
    
    return 0;
}