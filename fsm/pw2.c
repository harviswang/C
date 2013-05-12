//使用函数指针实现的基于状态机(FSM)的密码锁
//只有正确输入密码 2479 才能解锁 
#include <stdio.h>  
//这个秘密锁的密码是xxxx2479,就是说最后4位是2479,前面若干为为0~9里的数字,也可没有 
#include <stdlib.h>  
#include <string.h>   

//定义锁事件处理函数的函数指针类型
typedef void (*lock_func_temp)(char c);
typedef lock_func_temp (*lock_func)(char c);
lock_func state;

//函数声明队列 
//列出来,交叉引用才不会报错 
lock_func init_state(char ch);
lock_func state1(char ch);
lock_func state2(char ch);
lock_func state3(char ch);
lock_func state4(char ch);

//初始状态 
lock_func init_state(char ch)
{
    if ((ch < '0') || (ch > '9'))
        return NULL;	
    else
    	return state1(ch); //这里还必须得有参数,不然就会漏处理一个字符 
}

//状态1 
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

//状态2 
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

//状态3 
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

//状态4 
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

//结束状态是NULL
//就是通过 return NULL;表达的结束状态. 
 
//状态转换在这里 
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

