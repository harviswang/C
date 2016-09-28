/*
 * continue 在嵌套循环中
 */
#include <stdio.h>

#define DEBUG() printf("%s:%d\n", __func__, __LINE__)
#define PRINT(x) printf(#x"=%d\n", x)
static void continue_loop_nesting_test();
int main(int argc, char *argv[])
{
    printf("continue test ...\n");
    continue_loop_nesting_test();

    return 0;
}

static void continue_loop_nesting_test()
{
    int i, j, k;
    const int max = 3;
    
    for (i = 0; i < max; i++) {
        DEBUG();
        PRINT(i);
        continue;
        for (j = 0; j < max; j++) {
            DEBUG();
            PRINT(j);
            continue;
            for (k = 0; k < max; k++) {
                DEBUG();
                PRINT(k);
                continue;
            }
        }
    }
}