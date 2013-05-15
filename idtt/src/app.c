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
//----idtt�ն�����Ӧ��֡����----------------------------------------------------
//����:
//����:
//����: ��
//----------------------------------------------------------------------------
u32 idtt_send_response(u8 port_num,
                       u8 repeater_data,  u32 rtua, u16 mstaseq, u8 cmd, u32 timeout)
{
	u16 len;
	
	
	len = sizeof(u8) + sizeof(u8);
	u8 data[len];
	//��data���и�ֵ����
	//���� 
	idtt_send_data_base(data, len, rtua, mstaseq, cmd, timeout);
}
//----idtt��վ����֡����----------------------------------------------------
//����:
//����:
//����: ������˳��,��ι涨,��������һ�� 
//----------------------------------------------------------------------------
u32 idtt_send_repeater (u8 port_num, //�⼸�����ݵ�������ʲô? 
                        u32 timeout,
						u8 feature_byte,
						u8 get_from,
						u8 get_length,
						u8 n1nm,
						, u32 rtua, u16 mstaseq, u8 cmd, u32 timeout) 
{
	 u16 len;
	 u8 *data; 
	 //������վ����֡���ݶεĶ���,���������
	 len = sizeof(u8) + sizeof(u32);
	 //�������и��������ݶ���ϳ�һ���������ݶ�
	data = m_malloc(len, 0);
	idtt_send_data_base(data, len, rtua, mstaseq, cmd, timeout);
	m_free(data);
}
//----idtt��app����һ����֡����----------------------------------------------------
//����:
//����:
//����: ����ֵ��ι涨��????
//----------------------------------------------------------------------------
u32 idtt_send_data_base(u8 *data, u16 len, u32 rtua, u16 mstaseq, u8 cmd, u32 timeout)
{
	u8 *buf;
	u32 length;
	u8 head = 0x68;
	u8 cs; //У����, �Լ��㰡
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
	//���,��������ϳɻ���֡���ַ�����ʽ,���ͳ�ȥ
	memcpy(buf, &head, 1);
	memcpy(buf+1, (u8*)&rtua, sizeof(rtua));
	memcpy(buf+1+4, (u8*)&mstaseq, sizeof(mstaseq));
	memcpy(buf+1+4+2, &head, 1);
	memcpy(buf+1+4+2+1, &cmd, 1);
	memcpy(buf+1+4+2+1+1, (u8*)&len, sizeof(len));
	memcpy(buf+1+4+2+1+1+2, data, length);
	//���������һ��У����CS��ֵ
	for (cs = 0, i = 0; i < 1+4+2+1+1+2+len; i++)
	{
		cs = cs + buf[i];
	}
	cs = cs % 256; //��һ���ǲ���, �����˰�
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
//��֡��������Ϣ�������� 
u32 idtt_unpack_frame(u8 *frame, u32 len, struct IDTT_frame *result)
{
	u32 i;
	for (i = 0; i < len; i++)
	{	//���֡ͷ 
		if (frame[i] == 0x68)
			break; 
	} 
	if (i == 0 || i == len)
		return -1; //û��֡ͷ
	//��μ��֡����������
	//����У�����ж�һ��, ������������ж� 
	//�ж��ǵ�һ��֡ͷ,���ǵڶ�����ͷ
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

