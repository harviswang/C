/*
 * static 变量存储在函数作用域里面,要比存储在文件作用域安全的多
 * 通过函数调用的形式读写static变量实现了封装
 */
#include <stdio.h>
#include <assert.h>

int getA(int **addr)
{
    static int ABC = 1024;
    
    if (addr != NULL) {
        *addr = &ABC;
    }
    
    return ABC;
}

int setA(int a)
{
    int old_a;
    int *pA = NULL;
    
    old_a = getA(&pA);
    *pA = a;
    
    return old_a;
}

int main(int argc, char **argv)
{
    int i;
    
    for (i = 0; i < 1024; i++) {
        int old_a = setA(i);
        if (i != 0) {
            assert(old_a == i - 1);
        }
    }
    
    return 0;
}