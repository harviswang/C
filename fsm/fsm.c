//����ָ����������״̬�� 
#include <stdio.h>
int a () {
    return 2;
}
int b () {
    return 3;
}

//����ָ������
int (*func[])() = {a, b};

main ()
{
    printf("%d\n", func[0]());//���Թ��� 
    printf("%d\n", (*func[1])());//Ҳ���Թ���
    printf("%d\n", (*****func[1])());//��Ҳ��������,̫������ 
}
