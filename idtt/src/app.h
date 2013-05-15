#ifndef _app_H
#define _app_H
typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;

#define m_malloc(x, y) malloc((x))
#define m_free(x) free((x))
#ifndef true
#define true 1
#define false 0
#endif
//IDTT_frame֡�Ľṹ
struct IDTT_frame 
{
	u8 HEAD;
	u32 RTUA;
	u16 MSTASEQ;
	u8 HEAD2;
	u8  C;
	u32 L;
	u8 *DATA;
	u8 CS;
	u8 END;
};
struct IDTT_item 
{
	struct IDTT_frame frame;
	u32 len;
};

struct IDTT_list
{
	struct IDTT_item item;
	struct IDTT_list *next;
	struct IDTT_list *previous;
};
u32 idtt_send_data_base(u8 *data, u16 len, u32 rtua, u16 mstaseq, u8 cmd, u32 timeout);
void idtt_send_frame(u8 *data, u32 len, u32 timeout);
u32 idtt_pack_frame(u8 *data, u16 len, u32 rtua, u16 mstaseq, u8 cmd, struct IDTT_frame *result);
u32 idtt_unpack_frame(u8 *frame, u32 len, struct IDTT_frame *result);

#endif 
