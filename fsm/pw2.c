//ʹ�ú���ָ��ʵ�ֵĻ���״̬��(FSM)��������
//ֻ����ȷ�������� 2479 ���ܽ��� 
#include <stdio.h>  
//�����������������xxxx2479,����˵���4λ��2479,ǰ������ΪΪ0~9�������,Ҳ��û�� 
#include <stdlib.h>  
#include <string.h>   

//�������¼��������ĺ���ָ������
typedef void (*lock_func_temp)(char c);
typedef lock_func_temp (*lock_func)(char c);
lock_func state;

//������������ 
//�г���,�������òŲ��ᱨ�� 
lock_func init_state(char ch);
lock_func state1(char ch);
lock_func state2(char ch);
lock_func state3(char ch);
lock_func state4(char ch);

//��ʼ״̬ 
lock_func init_state(char ch)
{
    if ((ch < '0') || (ch > '9'))
        return NULL;	
    else
    	return state1(ch); //���ﻹ������в���,��Ȼ�ͻ�©����һ���ַ� 
}

//״̬1 
lock_func state1(char ch)
{
	if (ch == '2')
	{
		return state2;		
	} else 
	{
		return init_state;
	}
}

//״̬2 
lock_func state2(char ch)
{
	if (ch == '4')
	{
		return state3;		
	} else 
	{
		return init_state;
	}	
}

//״̬3 
lock_func state3(char ch)
{
	if (ch == '7')
	{
		return state4;		
	} else 
	{
		return init_state;
	}
}

//״̬4 
lock_func state4(char ch)
{
	if (ch == '9')
	{
		printf("Correct, lock is open!\n"); 
		return NULL;		
	} else 
	{
		return init_state;
	}
}

//����״̬��NULL
//����ͨ�� return NULL;���Ľ���״̬. 
 
//״̬ת�������� 
void lock_handle (void)
{
	char ch;
	state = init_state;
	while (state)
	{
		ch = getchar();
		state = (*state)(ch);
	}
} 

 
int main()   
{     	
	lock_handle();
} 

