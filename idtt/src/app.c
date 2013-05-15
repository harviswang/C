#include "app.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


main()
{
	u8 input[] = {1, 2, 4};
	struct IDTT_frame aoe;
	//idtt_send_data_base(input, 3, 0x01020304, 0x0506, 0x04, 0);
	idtt_pack_frame(input, 3, 0x01020304, 0x0506, 0x04, &aoe);
	//idtt_unpack_frame();
	printf("%d", aoe.CS);

}
//----idtt终端正常应答帧发送----------------------------------------------------
//功能:
//参数:
//返回: 返
//----------------------------------------------------------------------------
u32 idtt_send_response(u8 port_num,
                       u8 repeater_data,  u32 rtua, u16 mstaseq, u8 cmd, u32 timeout)
{
	u16 len;
	
	
	len = sizeof(u8) + sizeof(u8);
	u8 data[len];
	//对data进行赋值操作
	//发送 
	idtt_send_data_base(data, len, rtua, mstaseq, cmd, timeout);
}
//----idtt主站请求帧发送----------------------------------------------------
//功能:
//参数:
//返回: 参数的顺序,如何规定,还得斟酌一下 
//----------------------------------------------------------------------------
u32 idtt_send_repeater (u8 port_num, //这几个数据的类型是什么? 
                        u32 timeout,
						u8 feature_byte,
						u8 get_from,
						u8 get_length,
						u8 n1nm,
						, u32 rtua, u16 mstaseq, u8 cmd, u32 timeout) 
{
	 u16 len;
	 u8 *data; 
	 //根据主站请求帧数据段的定义,计算出长度
	 len = sizeof(u8) + sizeof(u32);
	 //将参数中各样子数据段组合成一个整的数据段
	data = m_malloc(len, 0);
	idtt_send_data_base(data, len, rtua, mstaseq, cmd, timeout);
	m_free(data);
}
//----idtt从app发送一基础帧数据----------------------------------------------------
//功能:
//参数:
//返回: 返回值如何规定呢????
//----------------------------------------------------------------------------
u32 idtt_send_data_base(u8 *data, u16 len, u32 rtua, u16 mstaseq, u8 cmd, u32 timeout)
{
	u8 *buf;
	u32 length;
	u8 head = 0x68;
	u8 cs; //校验码, 自己算啊
	u8 end = 0x16;
	u8 i;

	length = len +   sizeof(head) + 
	                 sizeof(rtua) + 
					 sizeof(mstaseq) + 
					 sizeof(head) + 
					 sizeof(cmd) + 
					 sizeof(u16) +
					 sizeof(cs) +
					 sizeof(end);
	buf = (u8 *)m_malloc(length, timeout);
	if (buf == NULL)
		return -1;
	//打包,将数据组合成基础帧的字符流格式,发送出去
	memcpy(buf, &head, 1);
	memcpy(buf+1, (u8*)&rtua, sizeof(rtua));
	memcpy(buf+1+4, (u8*)&mstaseq, sizeof(mstaseq));
	memcpy(buf+1+4+2, &head, 1);
	memcpy(buf+1+4+2+1, &cmd, 1);
	memcpy(buf+1+4+2+1+1, (u8*)&len, sizeof(len));
	memcpy(buf+1+4+2+1+1+2, data, length);
	//在这里计算一下校验码CS的值
	for (cs = 0, i = 0; i < 1+4+2+1+1+2+len; i++)
	{
		cs = cs + buf[i];
	}
	cs = cs % 256; //这一步是不是, 多余了啊
	memcpy(buf+1+4+2+1+1+2+len, &cs, 1);
	memcpy(buf+1+4+2+1+1+2+len+1, &end, 1);

	idtt_send_frame(buf, length, timeout);
	m_free((void *)buf);

	return length;
}
//
void idtt_send_frame(u8 *data, u32 len, u32 timeout)
{
	(void*)timeout;
	int i;
	for (i = 0; i < len; i++)
	{
		printf("%0.2x|", data[i]);
	}
	printf("\n");
}
//
u32 idtt_pack_frame(u8 *data, u16 len, u32 rtua, u16 mstaseq, u8 cmd, struct IDTT_frame *result)
{
	u8 cs;
	u16 i;
	
	result->HEAD = 0x68;
	result->RTUA = rtua;
	result->MSTASEQ = mstaseq;
	result->HEAD2 = 0x68;
	result->C = cmd;
	result->L = len;
	result->DATA = data;
	for (cs = 0, i = 0; i < len; i++)
		cs += data[i];
	cs += result->HEAD + 
	      *(u8 *)&result->RTUA + 
		  *((u8 *)&result->RTUA + 1)+
		  *((u8 *)&result->RTUA + 2)+
		  *((u8 *)&result->RTUA + 3)+
		  *((u8 *)&result->MSTASEQ + 0) +
		  *((u8 *)&result->MSTASEQ + 1) +
		  result->HEAD2 +
		  result->C +
		  *(u8*)result->L +
		  *((u8*)result->L + 1);
	
	result->CS = cs%256;
	result->END = 0x16;
	
	return true;
}
//将帧中有用信息解析出来 
u32 idtt_unpack_frame(u8 *frame, u32 len, struct IDTT_frame *result)
{
	u32 i;
	for (i = 0; i < len; i++)
	{	//监测帧头 
		if (frame[i] == 0x68)
			break; 
	} 
	if (i == 0 || i == len)
		return -1; //没有帧头
	//如何检测帧的完整性呢
	//根据校验码判断一下, 可以在最后面判断 
	//判断是第一个帧头,还是第二个枕头
	if (frame[i+4+2] != 0x68) 
		return -2;
	result->RTUA = frame[i+1] + 
	               frame[i+2]<<8 + 
				   frame[i+3]<<16 + 
				   frame[i+3]<<24;
	result->MSTASEQ = frame[i+4] + frame[i+5]<<8;
	result->C = frame[i+7];
	result->L = frame[i+8] + frame[i+9]<<8;
	result->DATA = &frame[i+10];
	result->CS = frame[i+result->L+11];
	if (frame[i+result->L+12] != 0x16)
		return -3;
	return result->L; 
}

