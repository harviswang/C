#include <stdio.h>

static void ternary_operator_defulat_test();
int main(int argc, char **argv)
{
    printf("ternary operator test\n");
    ternary_operator_defulat_test();
    
    return 0;
}

static void ternary_operator_defulat_test()
{
    int a = 8;
    int b = a ?/* nothing */: 9; /* same as 'int b = a ? a : 9;'*/
    /*int c = !a ? a : ; */ /* 语法错误 */
    
    printf("b = %d\n", b);
    /* printf("c = %d\n", c); */
}