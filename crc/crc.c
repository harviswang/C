#include <stdio.h> 
#include <string.h>
typedef char BYTE;
/*=========================================================
 计算CRC校验码函数;
	输  入:	要计算数据缓冲区的起始指针pBuff，缓冲区的字节数Len;
	输  出:	CRC校验码;
	功  能:	计算CRC校验码。
==========================================================*/
BYTE CRC(BYTE  *pBuff, int len)
{
	int	q, r;
	int	crc;
	int i;

	crc = 0;
	q = 0;		/*字节计数器*/
	r = 0;		/*字节内位的计数器*/
	while(q < len)
	{
		if(pBuff[q] & (0x80>>r))	/*当字节内某位为1时，在余数末尾置1，否则置0*/
			crc |= 0x01;
		if(crc >= 0x100)		
			crc ^= 0x107;		/*异或多项余子式*/
		crc <<= 1;
		r++;
		if(r == 8)			/*处理下一个字节*/
		{
			r = 0;
			q++;
		}
	}
	for(i=0; i<8; i++)		/*对位流的后8位计算CRC校验码*/
	{
		if(crc >= 0x100)
			crc ^= 0x107;
		crc <<= 1;
	}
	crc >>= 1;
	crc = (~crc);			/*余数取反*/
	return (BYTE)crc;
}

int main()
{ 
    char *in = "abc";
    
    printf("%d", CRC(in, strlen(in)));
}
