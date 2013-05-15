//֡�������Ժ���
//��������С�������: a. ǡ��һ֡
//                    b. ����һ֡
//                    c. ����һ֡
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "idtt.h"

//----֡��������-------------------------------------------------------
//����: �ѻ�����������ݰ���֡��ʽ��������
//����:
//      frame->last_state ->current_state����ʼ��ΪINIT״̬
//      frame->offset��ʼ��Ϊ0
//      frame->L��ʼ��Ϊ0
//����:
//-----------------------------------------------------------------------------
u32 frame_parse(u8 port_num, struct IDTT_frame *frame, char *input, u32 len)
{
	u32 i; //ָ����frame->DATA�е�λ��
    u8 cs = 0;

	switch(frame->last_state)
	{
		case INIT:
            printf("len = %d...\n", len);
            //����0x68,Ȼ��input������68H��ͷ�����ݸ��Ƶ�frame->DATA��, ���ƶ�pc
			//����״ֵ̬
			for (i = 0; i < len; i++)
				if(input[i] == 0x68)
					break;

			//�ж�һ����û���ҵ���Ϊͷ�ֶε�68H
			if ((i == 0 && input[i] == 0x68) ||  //ͷ�ֶ���input����λ��
			    (0 < i && i < len - 1) ||        //ͷ�ֶ���input���м�λ��
			    (i == len -1 && input[i] == 0x68))//ͷ�ֶ���input��β��λ��
			{
				// �������� ����->offset
				memcpy(frame->DATA + frame->offset, &input[i], 1); //ֻ����һ���ֽڵ�����
				frame->offset = 1;//->offsetָ����һ���洢���ݵĿ�ʼ��
				//״̬����
				frame->last_state = HEAD1;
				printf("HEAD1...\n");
				printf("offset = %d...\n", frame->offset);
				frame_parse(port_num, frame, input + i + 1, len - i - 1);
			} else //û�ҵ�HEAD0x68H, �����޷�������ȥ,
                break;
			break;
		case HEAD1:
            printf("\nlen = %d...\n", len);
			//�����û��HEAD2
			for (i = 0; i < len; i++)
				if(input[i] == 0x68)
					break;
			//�ж�һ����û���ҵ���Ϊͷ�ֶε�68H
			if ((i == 0 && input[i] == 0x68) ||  //ͷ�ֶ���input����λ��
			    (0 < i && i < len - 1) ||        //ͷ�ֶ���input���м�λ��
			    (i == len -1 && input[i] == 0x68))//ͷ�ֶ���input��β��λ��
			{	//�ҵ�HEAD2
				memcpy(frame->DATA + frame->offset, input, frame->R_left);
                printf("%02x\n", i);
                frame->offset += frame->R_left;
                frame->last_state = HEAD2;
                printf("HEAD2...\n");
                printf("offset = %d...\n", frame->offset);
				frame_parse(port_num, frame, input+i+1, len - frame->R_left);
			}
			else
			{	//û�ҵ�HEAD2, ״̬�򲻱�, ��input���ݽ�����frame->DATA[offset]���
				//׷�Ӹ���
				memcpy(frame->DATA + frame->offset, input, len);
				frame->offset += len;
				frame->R_left = frame->R_left - len;
            }
			break;
		case HEAD2:
            printf("\nlen = %d...\n", len);
			//�ҿ�����
			if(len > 0)//���ȴ���0, ��Ϊ�п�����
            {
                memcpy(frame->DATA + frame->offset, input, 1);;//���ƿ����뵽->DATA
                frame->offset += 1;
                frame->last_state = C;
                printf("C...\n");
                printf("offset = %d...\n", frame->offset);
                frame_parse(port_num, frame, input+1, len - 1);
            }

			break;
		case C:
            //�ҳ����ֶ�L(16bits)
            printf("\nlen = %d...\n", len);

            if(len >= 2)
            {
                if(frame->L_left == 0) //����L��û�п�ʼ����
                {
                    memcpy(frame->DATA + frame->offset, input, 2);;//���Ƴ���L��->DATA
                    frame->offset += 2;
                    frame->last_state = L;
                    printf("L...\n");
                    printf("offset = %d...\n", frame->offset);
                    frame->L += ((u16)input[0])|(((u16)input[1])<<8); //ԭ����Ҫ�û�����
                    frame->L_left = frame->L;
                    printf("L_left = %02x...\n", frame->L_left);
                    printf("L[0] = %x\n", input[0]);
                    printf("L[1] = %x\n", input[1]);
                    frame_parse(port_num, frame, input+2, len - 2);
                } else {
                    //L�Ѿ���������һ���ֽ�
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
                //������������, ������״̬
                printf("L[0] = %x\n", input[0]);
                memcpy(frame->DATA + frame->offset, input, 1);
                frame->offset += 1;
                if(frame->L_left == 0)
                {
                    frame->L += input[0]; //���ﲻ��???
                    frame->L_left += frame->L;
                }
                else
                {
                    frame->L |= input[0]<<8; //���ﲻ��???
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
            //�����������ֶ�����
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
            //����У���
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
            //����END�ֶ�
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
			//�����ݰ���struct idtt_frame�Ľṹ��ʽ����ȥ, ��App��ʹ��
			//��DATA��ȡ����,�ֱ�Ϊstruct idtt_frame�ĸ�����Ա
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
            //����У����
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
            //�����ﴦ�����¼��ľ������
            for(i = 0; i < frame->L; i++)
                printf("%c ", frame->DATA[i]);
                //printf("%02x ", frame->DATA[i]);
            //��λframe�ĳ�ʼֵ, ������һ��֡ʹ��
            frame->L=0;
            frame->last_state = INIT;
            frame->offset=0;
            frame->L_left = 0;
			break;
	}

	return 0;
}
