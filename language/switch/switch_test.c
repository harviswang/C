#include <stdio.h>

int main()
{
    int xxx = 0;

    switch(xxx) {
    case -1:
        printf("-1\n");
        break;
    case 0:
        printf("0\n");
    case 1:
        printf("1\n");
        break;
    default:
        printf("unknow\n");
    }

    return 0;
}
