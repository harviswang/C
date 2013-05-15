//帧解析测试函数
//缓冲区大小三种情况: a. 恰好一帧
//                    b. 不够一帧
//                    c. 多于一帧
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "idtt.h"

//----帧解析函数-------------------------------------------------------
//功能: 把缓冲区里的数据按照帧格式解析出来
//参数:
//      frame->last_state ->current_state均初始化为INIT状态
//      frame->offset初始化为0
//      frame->L初始化为0
//返回:
//-----------------------------------------------------------------------------
u32 frame_parse(u8 port_num, struct IDTT_frame *frame, char *input, u32 len)
{
	u32 i; //指针在frame->DATA中的位置
    u8 cs = 0;

	switch(frame->last_state)
	{
		case INIT:
            printf("len = %d...\n", len);
            //查找0x68,然后将input里面以68H开头的数据复制到frame->DATA中, 并移动pc
			//更新状态值
			for (i = 0; i < len; i++)
				if(input[i] == 0x68)
					break;

			//判断一下有没有找到作为头字段的68H
			if ((i == 0 && input[i] == 0x68) ||  //头字段在input的首位置
			    (0 < i && i < len - 1) ||        //头字段在input的中间位置
			    (i == len -1 && input[i] == 0x68))//头字段在input的尾巴位置
			{
				// 拷贝数据 更新->offset
				memcpy(frame->DATA + frame->offset, &input[i], 1); //只复制一个字节的数据
				frame->offset = 1;//->offset指向下一个存储数据的开始处
				//状态更新
				frame->last_state = HEAD1;
				printf("HEAD1...\n");
				printf("offset = %d...\n", frame->offset);
				frame_parse(port_num, frame, input + i + 1, len - i - 1);
			} else //没找到HEAD0x68H, 解析无法进行下去,
                break;
			break;
		case HEAD1:
            printf("\nlen = %d...\n", len);
			//检测有没有HEAD2
			for (i = 0; i < len; i++)
				if(input[i] == 0x68)
					break;
			//判断一下有没有找到作为头字段的68H
			if ((i == 0 && input[i] == 0x68) ||  //头字段在input的首位置
			    (0 < i && i < len - 1) ||        //头字段在input的中间位置
			    (i == len -1 && input[i] == 0x68))//头字段在input的尾巴位置
			{	//找到HEAD2
				memcpy(frame->DATA + frame->offset, input, frame->R_left);
                printf("%02x\n", i);
                frame->offset += frame->R_left;
                frame->last_state = HEAD2;
                printf("HEAD2...\n");
                printf("offset = %d...\n", frame->offset);
				frame_parse(port_num, frame, input+i+1, len - frame->R_left);
			}
			else
			{	//没找到HEAD2, 状态则不变, 将input数据近挨着frame->DATA[offset]存放
				//追加复制
				memcpy(frame->DATA + frame->offset, input, len);
				frame->offset += len;
				frame->R_left = frame->R_left - len;
            }
			break;
		case HEAD2:
            printf("\nlen = %d...\n", len);
			//找控制码
			if(len > 0)//长度大于0, 认为有控制码
            {
                memcpy(frame->DATA + frame->offset, input, 1);;//复制控制码到->DATA
                frame->offset += 1;
                frame->last_state = C;
                printf("C...\n");
                printf("offset = %d...\n", frame->offset);
                frame_parse(port_num, frame, input+1, len - 1);
            }

			break;
		case C:
            //找长度字段L(16bits)
            printf("\nlen = %d...\n", len);

            if(len >= 2)
            {
                if(frame->L_left == 0) //长度L还没有开始计算
                {
                    memcpy(frame->DATA + frame->offset, input, 2);;//复制长度L到->DATA
                    frame->offset += 2;
                    frame->last_state = L;
                    printf("L...\n");
                    printf("offset = %d...\n", frame->offset);
                    frame->L += ((u16)input[0])|(((u16)input[1])<<8); //原来是要用或运算
                    frame->L_left = frame->L;
                    printf("L_left = %02x...\n", frame->L_left);
                    printf("L[0] = %x\n", input[0]);
                    printf("L[1] = %x\n", input[1]);
                    frame_parse(port_num, frame, input+2, len - 2);
                } else {
                    //L已经被计算了一个字节
                    memcpy(frame->DATA + frame->offset, input, 1);
                    frame->offset += 1;
                    frame->last_state = L;
                    frame->L += input[0];
                    frame->L_left = frame->L;
                    printf("L...\n");
                    printf("offset = %d...\n", frame->offset);
                    printf("L[0] = %x\n", input[0]);
//                    printf("L_left = %02x...\n", frame->L_left);
//                    printf("L = %02x...\n", frame->L);
                    frame_parse(port_num, frame, input+1, len - 1);
                }
            } else if(len == 1)
            {
                //仅仅拷贝数据, 不更新状态
                printf("L[0] = %x\n", input[0]);
                memcpy(frame->DATA + frame->offset, input, 1);
                frame->offset += 1;
                if(frame->L_left == 0)
                {
                    frame->L += input[0]; //这里不对???
                    frame->L_left += frame->L;
                }
                else
                {
                    frame->L |= input[0]<<8; //这里不对???
                    frame->L_left = frame->L;
                    frame->last_state = L;
                }
                printf("offset = %02x...\n", frame->offset);
                printf("L_left = %02x...\n", frame->L_left);
            }
//            printf("L = %02x...\n", frame->L);
            break;
		case L:
            printf("\nlen = %d...\n", len);
            //复制数据域字段内容
            if(frame->L_left <= len)
            {
                memcpy(frame->DATA + frame->offset, input, frame->L_left);
                frame->offset += frame->L_left;
                frame->last_state = DATA;
         //       frame->L_left = 0;
                printf("DATA...\n");
                printf("offset = %d...\n", frame->offset);

                frame_parse(port_num, frame, input+frame->L_left, len - frame->L_left);
            }
            else
            {
                memcpy(frame->DATA + frame->offset, input, len);
                frame->offset += len;
                frame->L_left -= len;
            }
			break;
		case DATA:
            printf("\nlen = %d...\n", len);
            //复制校验段
            if(len > 0)
            {
                memcpy(frame->DATA + frame->offset, input, 1);
                frame->offset += 1;
                frame->last_state = CS;
                printf("CS...\n");
                printf("offset = %d...\n", frame->offset);
                frame_parse(port_num, frame, input+1, len - 1);
            }
			break;
		case CS:
            printf("\nlen = %d...\n", len);
            //复制END字段
            if(len > 0)
            {
                memcpy(frame->DATA + frame->offset, input, 1);
                frame->offset += 1;
                frame->last_state = END;
                printf("END...\n");
                printf("offset = %d...\n", frame->offset);
                frame_parse(port_num, frame, input+1, len - 1);
            }
			break;
		case END:
			//把数据按照struct idtt_frame的结构形式弹出去, 供App层使用
			//从DATA中取数据,分别为struct idtt_frame的各个成员
			frame->HEAD = frame->DATA[0];
			printf("HEAD1 = %02x\n", frame->HEAD);
			frame->RTUA = frame->DATA[1] |
                          frame->DATA[2]<<8 |
                          frame->DATA[3]<<16 |
                          frame->DATA[4]<<24;
            printf("RTUA = %08x\n", frame->RTUA);
            frame->MSTASEQ = frame->DATA[5] | frame->DATA[6]<<8;
            printf("MSTASEQ = %04x\n", frame->MSTASEQ);
            frame->HEAD2 = frame->DATA[7];
            printf("HEAD2 = %02x\n", frame->HEAD2);
            frame->C = frame->DATA[8];
            printf("C = %01x\n", frame->C);
            printf("L = %02x\n", frame->L);
            frame->CS = frame->DATA[11 + frame->L];
            printf("CS = %01x\n", frame->CS);
            frame->END = frame->DATA[12 + frame->L];
            printf("END = %01x\n", frame->END);
            //计算校验码
            for(i = 0, cs = 0; i < frame->L + 11; i++)
                cs += frame->DATA[i];
            cs = cs%256;
            printf("cs = %x\n", cs);
            if(cs != frame->CS)
            {
                printf("invalid frame, will be give up\n");
            }
            else
            {
                for(i = 0; i < frame->L; i++)
                    frame->DATA[i] = frame->DATA[11+i];
            }
            //在这里处理弹出事件的具体操作
            for(i = 0; i < frame->L; i++)
                printf("%c ", frame->DATA[i]);
                //printf("%02x ", frame->DATA[i]);
            //复位frame的初始值, 解析下一个帧使用
            frame->L=0;
            frame->last_state = INIT;
            frame->offset=0;
            frame->L_left = 0;
			break;
	}

	return 0;
}
