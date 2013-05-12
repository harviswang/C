//将数据类型的大小,通过指针传到函数中
//有点遗憾,这种办法没法在ARM等需要内存对齐的场合使用 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//在返回的函数名之前,加个强制类型转换,可以消除警告, 
//协议相关的类型,注意全部是无符号类型,有符号容易出麻烦.不同的编译器,可能还不同. 
typedef unsigned int u32;
typedef unsigned short int u16;
typedef unsigned char u8;

//定义指向函数的指针类型. 
typedef u32 (*protocol_func)(void *, u8 *);
typedef protocol_func (*func_type)(void *, u8 *);

func_type init_state(void *ch, u8 *type_size);
func_type head_state(void *ch, u8 *type_size);
func_type type_state(void *ch, u8 *type_size);
func_type data_length_state(void *ch, u8 *type_size);
func_type data_state(void *ch, u8 *type_size);
func_type cs_state(void *ch, u8 *type_size);
func_type end_state(void *ch, u8 *type_size);

//用户回调函数
//存放解析出来的数据内容,以及数据的长度
void data_copy(void *dest, void *src, int len); //简单的讲数据复制走
unsigned char repository[256];//仓库,用户回调函数处理后的结果,存储在这里面 
unsigned char length = 0, ip = 0;//length是状态转换用的, ip是仓库的容量 
//unsigned char TYPE_SIZE = 0; //这个记录状态对应的类型的大小,这个地方处理的不大好,没想到更好的方法. 

//用户自定义的,处理解包出来的数据,相当于一个回调函数 
void data_copy(void *dest, void *src, int len)
{
	memcpy(dest, src, len);
	//做其它的处理.... 
}

//状态机初始状态 
func_type init_state(void *ch, u8 *type_size)
{
	if (ch == NULL)
       return NULL;
	else{
		*type_size = 0;
		printf("init state\n");
		return (func_type)head_state;	
	}
	   
}

//帧HEAD状态
//cp表示扫描指针的当前位置
//type_size表示此状态字段在帧结构中的字节数 
func_type head_state(void *ch, u8 *type_size)
{
	if (*(u16 *)ch == 0xa5a5)
	{
       	//做点处理 
       	*type_size = 2;
		printf("head state\n");
	   	return (func_type)type_state;//进入data length状态 
	}else
	{
		*type_size = 1; //去下一个字节,检查帧头表示符 
		return (func_type)init_state;
	}
	   	
}

//帧TYPE状态
func_type type_state(void *ch, u8 *type_size)
{
	if (*(u8 *)ch >= 0x00 && *(char *)ch <= 0xff)
	{
       //做点处理 
       *type_size = 1;
       printf("type state\n");
	   return (func_type)data_length_state;//进入data length状态 
	}else
	   return (func_type)init_state;	
}

//帧DATA LENGTH状态
func_type data_length_state(void *ch, u8 *type_size)
{
	if (*(u8 *)ch >= 0x00 && *(u8 *)ch <= 0xff)//这个判断看上去,是多余的,起提示类型u8的作用. 
	{
       //做点处理 
       *type_size = 1;
	   length = *(char *)ch; //记录数据长度,因为下一个状态要用
	   printf("data lenth state\n");
	   return (func_type)data_state;//进入data状态 
	}else
	   return (func_type)init_state;	
}

//帧DATA状态
//*type_size表示数据段的长度 
func_type data_state(void *ch, u8 *type_size)
{
	if (ch != NULL)
	{
		//做点处理
		//调用用户回调函数,复制数据
		//此时指针ch,应该指向数据段的第一个字节除,怎么做到呢
		ip = length;
		length = 0;

		*type_size = ip;
		data_copy(repository, ch, ip);
		printf("data state\n");
		return (func_type)cs_state;//进入cs状态 
	}else
		return (func_type)init_state;	
}

//帧CS状态,具体的校验并未处理 
//
func_type cs_state(void *ch, u8 *type_size)
{
	if (ch != NULL)
	{
       //做点处理 
       printf("cs state\n");
       *type_size = 1;
	   return (func_type)end_state;//进入end状态 
	}else
	   return (func_type)init_state;	
}

//帧End状态 
func_type end_state(void *ch, u8 *type_size)
{
	if (*(u16 *)ch == 0xbeef)
	{
       //做点处理 
       printf("end state\n");
       *type_size = 2;
	   return NULL;//进入状态 
	}else
	   return (func_type)init_state;	
}

//状态机的运行函数 
//参数应该是一个指针 
//指针怎么变呢
void run_state(void *ch, u16 len)
{
	func_type state;
	state = (func_type)init_state;
	u16 i = len;// = 11; //我测试的时候用的,有bug,容易死循环 
	            //在这里根据数据的长度,做个限制,避免包的格式错误导致死循环 
	u8 type_size = 0;
	while (state && i > 0)
	{
		printf("[%x] --- [%d]\n", (u32)ch, i);
		state = (*state)(ch, &type_size);
		ch += type_size;//取到下一个指针
		i--;
	}
}

//将用户给的数据打包,包存放的地方也是由用户决定,以及包的大小 
u8 make_pack(void *data, u8 length, u8 type, u8 *buf, u8 *len)
{
	u8 cs, i;
	u8 head[] = {0xa5, 0xa5};
	u16 end[] = {0xef, 0xbe}; 
	//buf = (u8 *)malloc(len + 7); //7是协议帧除掉DATA后的长度,那是固定的
	                               //如果在这分配内存,必然导致内存泄漏.
 	if (buf == NULL) 
 	{
 		printf("malloc error\n");
	    exit(1);
 	}
 	//计算校验码,type、data length、data三个域所有字节的异或值 
 	cs = head[0]^head[1]^length;
 	for (i = 0; i < length; i++)
 	{
 		cs = cs ^ *(u8*)(data + i);
 	}
 	//将数据装入帧格式规定了的形式 
 	memcpy(buf, (void *)head, 2);
	memcpy(buf+2, &type, 1);
	memcpy(buf+3, &length, 1);
	memcpy(buf+4, data, length);
	memcpy(buf+4+length, &cs, 1);
	memcpy(buf+4+length+1, &end, 2);
	
	if (len != NULL)
		*len = 4+length+1+2;
	else
		return 0;
	return 1;
}
int main()
{
	u8 good[] = {0xa5, 0x00, 0xa5, 0xa5, 0x22, 0x04, 0x38, 0x01, 0x03, 0x42, 0x00, 0xef, 0xbe};
	u8 arr[]={1,2,3} ;
	u8 buff[1024];
	u8 len=0;
	u8 type = 0x34;
	
	run_state((void *)good, sizeof(good)/sizeof(good[0]));
	
	//printf("%x\n", *(u16 *)&good[7]);
	//printf("%x\n", *(u16 *)&good[1]);
	printf("%x\n", *(u16*)&repository[1]);
	printf("%x\n", ip);
	make_pack(arr, 3, type, buff, &len);
	
	run_state((void *)buff, len);
	printf("[%d]\n", len);
	printf("%x\n", *(u16*)&repository[0]);
	printf("%x\n", ip);
}
