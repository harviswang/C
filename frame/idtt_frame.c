#include <stdio.h>

#define cn_rs485_send_number 64
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

struct idtt_msg
{
	u32 num;
	u32 total;
	char  input_char[128];
};

//IDTT֡�Ľṹ
struct IDTT_frame
{
	u8 HEAD;
	u32 RTUA;
	u16 MSTASEQ;
	u8 HEAD2;
	u8  C;
	u32 L;
	u8 DATA[cn_rs485_send_number - 13];
	u8 CS;
	u8 END;
};
//----idtt�¼�����ɨ�躯��-------------------------------------------------------
//����: �����Ӵ��ڽ��յ������ݰ���֡��ʽ������, �������ϲ�Ӧ��
//����:
//����:
//-----------------------------------------------------------------------------
void idtt_recv_scan(void)
{
    struct idtt_msg msg; //����ṹ����Ҫ���Զ����,����������ѽ
    struct IDTT_frame frame;


    u32 i, pointer;
    u32 rtua;
    u16 mstaseq, len;
    u8 c, cs, cs2, end; //cΪ��֡�н�������У����, csΪ����֡���ݼ��������У����
    while(1)
    {
    	msg.total = 0;
    	msg.num = 0; //������λ, ����б�Ҫ��???
    	pointer = -1;
        do
        {
            //���ն��豸, ÿ������Ҫ��13�ֽ�, ��Ϊ��֡��������Ϊ13Bytes, �ƺ�û����
        	//����13�ֽ�ҲҪ���� Ҫ��Ҫ���������ø��˿�ת����
            msg.num = dev_read_left(idtt_lhdl,0,
                                    (ptu32_t)(msg.input_char + msg.total), cn_rs485_recv_limit);
            msg.total += msg.num;
//            if(msg.total >= 13)
            //��������֤֡, ����֡
            for (i = 0; i < msg.total; i++)
            {
            	//���HEAD�ֶ�
            	if(msg.input_char[i] == 0x68)
            	{
            		uart_debug_print("HEAD...", 7);
            		pointer = i;
            	}
            }
            //û�м���HEAD�ֶ�, ֱ���˳�do whileѭ��
            if(pointer == -1)
            	break;
            //input_char[]��������һ���ֽ��������ݸպ���0x68զ��???
            pointer = pointer + 1;//ָ������һ���ֽ�
            if (pointer + 3 > msg.total) break; //���Խ��զ�� ???
            //��ȡRTUA�ֶ� �������ĸ����˳����û��������
            rtua = msg.input_char[pointer + 0] << 24 |
            	   msg.input_char[pointer + 1] << 16 |
            	   msg.input_char[pointer + 2] << 8  |
            	   msg.input_char[pointer + 3] ;
            uart_debug_print("RTUA�ֶ�", 8);
            pointer = pointer + 4;//ָ������4���ֽ�
            if (pointer + 1 > msg.total) break;
            //��ȡMSTASEQ�ֶ�
            mstaseq = msg.input_char[pointer + 0] << 8 |
            		  msg.input_char[pointer + 1] ;
            uart_debug_print("MSTASEQ�ֶ�", 11);
            //���֡��Ŀ�ĵ�ַ�Ƿ����Լ�, ���ǾͲ�Ҫ���´�����, ���������ƺ�����,
            //����һ֡���������, ����Щ�˷�ʱ��
            pointer = pointer + 2;//ָ������2���ֽ�
//            if (pointer > msg.total) break;
            //���ڶ���HEAD�ֶ�
        	if(msg.input_char[pointer] == 0x68)
        	{
        		uart_debug_print("HEAD2...", 8);
        	}
//        	else
//        		break; //��˴��������
        	uart_debug_print("HEAD2", 5);
        	pointer = pointer + 1;//ָ������1���ֽ�
//        	if (pointer > msg.total) break;
        	//ȡ������C
        	c = msg.input_char[pointer];
        	uart_debug_print("������C", 7);
        	pointer = pointer + 1;//ָ������1���ֽ�
//        	if (pointer > msg.total) break;
        	//ȡ���ݳ���L
        	len = msg.input_char[pointer];
        	frame.L = len;
        	uart_debug_print("���ݳ���L", 9);
        	pointer = pointer + 1;//ָ������1���ֽ�
//        	if (pointer > msg.total) break;
        	//ȡDATA�ֶ�
        	memcpy(frame.DATA, msg.input_char, len);
        	uart_debug_print("DATA�ֶ�", 8);
        	pointer = pointer + len;//ָ������L���ֽ�
        	if (pointer > msg.total) break;
        	//ȡУ����
        	cs = msg.input_char[pointer];
        	uart_debug_print("У�����ֶ�", 10);
        	//У��������Ƿ���ȷ
        	for (i = 0, cs2 = 0; i < pointer; i++)
        		cs2 = cs2 + msg.input_char[i];
        	cs2 = cs2 % 256;
//        	if(cs != cs2) break; //ֱ�ӷ�������Ľ���
        	pointer = pointer + 1;//ָ������1���ֽ�
//			if (pointer > msg.total) break;
			//ȡ������־END
			end = msg.input_char[pointer];
//			if(end != 0x16) break;
			uart_debug_print("������־END", 11);
			//֡�Ǻϸ��
			//����Ƿ񷢸��Լ���
			//if(xx == me) then ...

			//��֡���ϲ㴫��
//			djy_event_pop(djy_get_evtt_id("idtt_recv_evtt"),NULL,0,(void *)&frame, sizeof(frame), 0, 0);
			print_hexstream((char *)msg.input_char, len);
			djy_event_delay(400*mS);
        } while (msg.num == cn_rs485_recv_limit); //ֱ�����е����ݶ�������
        //���յ�����������һ֡

        djy_wait_evtt_pop(djy_my_evtt_id(),1,cn_timeout_forever);
    }
}
