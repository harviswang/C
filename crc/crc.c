#include <stdio.h> 
#include <string.h>
typedef char BYTE;
/*=========================================================
 ����CRCУ���뺯��;
	��  ��:	Ҫ�������ݻ���������ʼָ��pBuff�����������ֽ���Len;
	��  ��:	CRCУ����;
	��  ��:	����CRCУ���롣
==========================================================*/
BYTE CRC(BYTE  *pBuff, int len)
{
	int	q, r;
	int	crc;
	int i;

	crc = 0;
	q = 0;		/*�ֽڼ�����*/
	r = 0;		/*�ֽ���λ�ļ�����*/
	while(q < len)
	{
		if(pBuff[q] & (0x80>>r))	/*���ֽ���ĳλΪ1ʱ��������ĩβ��1��������0*/
			crc |= 0x01;
		if(crc >= 0x100)		
			crc ^= 0x107;		/*����������ʽ*/
		crc <<= 1;
		r++;
		if(r == 8)			/*������һ���ֽ�*/
		{
			r = 0;
			q++;
		}
	}
	for(i=0; i<8; i++)		/*��λ���ĺ�8λ����CRCУ����*/
	{
		if(crc >= 0x100)
			crc ^= 0x107;
		crc <<= 1;
	}
	crc >>= 1;
	crc = (~crc);			/*����ȡ��*/
	return (BYTE)crc;
}

int main()
{ 
    char *in = "abc";
    
    printf("%d", CRC(in, strlen(in)));
}
