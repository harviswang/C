#ifndef _IDTT_H_
#define _IDTT_H_

typedef unsigned int u32;
typedef unsigned short int u16;
typedef unsigned char u8;

#define cn_rs485_send_number 64

//֡���������еĸ���״̬
enum idtt_state {INIT = 0, HEAD1, HEAD2, C, L, DATA, CS, END};

//IDTT֡�Ľṹ
struct IDTT_frame
{
	u8 HEAD;
	u32 RTUA;
	u32 R_left;
	u16 MSTASEQ;
	u8 HEAD2;
	u8  C;
	u16 L;
	u16 L_left;
	u8 DATA[cn_rs485_send_number - 13];
	u8 CS;
	u8 END;
	u16 offset; //��¼->DATA�ĵ�ǰƫ����
	enum idtt_state last_state; //�ϴδ�����״̬
//	enum idtt_state current_state; //
};

u32 frame_parse(u8 port_num, struct IDTT_frame *frame, char *input, u32 len);
void test1();
void test2();
void test3();
void test4();
void test5();
void test6();
void test7();
#endif
