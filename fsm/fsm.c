//函数指针数组描述状态机 
#include <stdio.h>
int a () {
    return 2;
}
int b () {
    return 3;
}

//函数指针数组
int (*func[])() = {a, b};

main ()
{
    printf("%d\n", func[0]());//可以工作 
    printf("%d\n", (*func[1])());//也可以工作
    printf("%d\n", (*****func[1])());//这也正常工作,太神奇了 
}
