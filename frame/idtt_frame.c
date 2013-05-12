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

//IDTT帧的结构
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
//----idtt事件类型扫描函数-------------------------------------------------------
//功能: 解析从串口接收到的数据按照帧格式调整好, 反馈给上层应用
//参数:
//返回:
//-----------------------------------------------------------------------------
void idtt_recv_scan(void)
{
    struct idtt_msg msg; //这个结构可能要换自定义的,容量不够大呀
    struct IDTT_frame frame;


    u32 i, pointer;
    u32 rtua;
    u16 mstaseq, len;
    u8 c, cs, cs2, end; //c为从帧中解析出的校验码, cs为根据帧内容计算出来的校验码
    while(1)
    {
    	msg.total = 0;
    	msg.num = 0; //容量复位, 这个有必要吗???
    	pointer = -1;
        do
        {
            //读终端设备, 每次至少要读13字节, 因为空帧的数据量为13Bytes, 似乎没道理啊
        	//不够13字节也要处理 要不要在这里设置个端口转换呢
            msg.num = dev_read_left(idtt_lhdl,0,
                                    (ptu32_t)(msg.input_char + msg.total), cn_rs485_recv_limit);
            msg.total += msg.num;
//            if(msg.total >= 13)
            //在这里验证帧, 解析帧
            for (i = 0; i < msg.total; i++)
            {
            	//检测HEAD字段
            	if(msg.input_char[i] == 0x68)
            	{
            		uart_debug_print("HEAD...", 7);
            		pointer = i;
            	}
            }
            //没有检测掉HEAD字段, 直接退出do while循环
            if(pointer == -1)
            	break;
            //input_char[]若仅仅有一个字节它的内容刚好是0x68咋办???
            pointer = pointer + 1;//指针下移一个字节
            if (pointer + 3 > msg.total) break; //如果越界咋办 ???
            //提取RTUA字段 下面这四个组合顺序有没有问题呢
            rtua = msg.input_char[pointer + 0] << 24 |
            	   msg.input_char[pointer + 1] << 16 |
            	   msg.input_char[pointer + 2] << 8  |
            	   msg.input_char[pointer + 3] ;
            uart_debug_print("RTUA字段", 8);
            pointer = pointer + 4;//指针下移4个字节
            if (pointer + 1 > msg.total) break;
            //提取MSTASEQ字段
            mstaseq = msg.input_char[pointer + 0] << 8 |
            		  msg.input_char[pointer + 1] ;
            uart_debug_print("MSTASEQ字段", 11);
            //检测帧的目的地址是否是自己, 不是就不要往下处理了, 在这里检查似乎不妥,
            //但在一帧解析完后检查, 又有些浪费时间
            pointer = pointer + 2;//指针下移2个字节
//            if (pointer > msg.total) break;
            //检测第二个HEAD字段
        	if(msg.input_char[pointer] == 0x68)
        	{
        		uart_debug_print("HEAD2...", 8);
        	}
//        	else
//        		break; //如此处理合理吗
        	uart_debug_print("HEAD2", 5);
        	pointer = pointer + 1;//指针下移1个字节
//        	if (pointer > msg.total) break;
        	//取控制码C
        	c = msg.input_char[pointer];
        	uart_debug_print("控制码C", 7);
        	pointer = pointer + 1;//指针下移1个字节
//        	if (pointer > msg.total) break;
        	//取数据长度L
        	len = msg.input_char[pointer];
        	frame.L = len;
        	uart_debug_print("数据长度L", 9);
        	pointer = pointer + 1;//指针下移1个字节
//        	if (pointer > msg.total) break;
        	//取DATA字段
        	memcpy(frame.DATA, msg.input_char, len);
        	uart_debug_print("DATA字段", 8);
        	pointer = pointer + len;//指针下移L个字节
        	if (pointer > msg.total) break;
        	//取校验码
        	cs = msg.input_char[pointer];
        	uart_debug_print("校验码字段", 10);
        	//校验控制码是否正确
        	for (i = 0, cs2 = 0; i < pointer; i++)
        		cs2 = cs2 + msg.input_char[i];
        	cs2 = cs2 % 256;
//        	if(cs != cs2) break; //直接放弃下面的解析
        	pointer = pointer + 1;//指针下移1个字节
//			if (pointer > msg.total) break;
			//取结束标志END
			end = msg.input_char[pointer];
//			if(end != 0x16) break;
			uart_debug_print("结束标志END", 11);
			//帧是合格的
			//检测是否发给自己的
			//if(xx == me) then ...

			//把帧往上层传递
//			djy_event_pop(djy_get_evtt_id("idtt_recv_evtt"),NULL,0,(void *)&frame, sizeof(frame), 0, 0);
			print_hexstream((char *)msg.input_char, len);
			djy_event_delay(400*mS);
        } while (msg.num == cn_rs485_recv_limit); //直到所有的数据都被读出
        //接收到的数据量够一帧

        djy_wait_evtt_pop(djy_my_evtt_id(),1,cn_timeout_forever);
    }
}
