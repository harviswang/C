#include <stdio.h>
#include <stdlib.h>

//Э����ص�����,ע��ȫ�����޷�������,�з������׳��鷳.��ͬ�ı�����,���ܻ���ͬ. 
typedef unsigned int u32;
typedef unsigned short int u16;
typedef unsigned char u8;

//����ָ������ָ������. 
typedef void (*protocol_func)(void *);
typedef protocol_func (*func_type)(void *);

func_type state;

func_type init_state(void *ch);
func_type head_state(void *ch);
func_type type_state(void *ch);
func_type data_length_state(void *ch);
func_type data_state(void *ch);
func_type cs_state(void *ch);
func_type end_state(void *ch);
void *get_next_point(void *ch, unsigned char size);
//�û��ص�����
//��Ž�����������������,�Լ����ݵĳ���
void data_copy(void *dest, void *src, int len); //�򵥵Ľ����ݸ�����
unsigned char repository[256];//�ֿ�,�û��ص����������Ľ��,�洢�������� 
unsigned char length = 0, ip = 0;//length��״̬ת���õ�, ip�ǲֿ������ 
unsigned char TYPE_SIZE = 0; //�����¼״̬��Ӧ�����͵Ĵ�С,����ط�����Ĳ����,û�뵽���õķ���. 

//�û��Զ����,����������������,�൱��һ���ص����� 
void data_copy(void *dest, void *src, int len)
{
	memcpy(dest, src, len);
	//�������Ĵ���.... 
}

//״̬����ʼ״̬ 
func_type init_state(void *ch)
{
	if (ch == NULL)
       return NULL;
	else{
		TYPE_SIZE = 0;
		printf("init state\n");
		return head_state;	
	}
	   
}

//֡HEAD״̬
func_type head_state(void *ch)
{
	if (*(u16 *)ch == 0xa5a5)
	{
       //���㴦�� 
       TYPE_SIZE = 2;
       	printf("head state\n");
	   return type_state;//����data length״̬ 
	}else
	{
		TYPE_SIZE = 1; //ȥ��һ���ֽ�,���֡ͷ��ʾ�� 
		return init_state;
	}
	   	
}

//֡TYPE״̬
func_type type_state(void *ch)
{
	if (*(u8 *)ch >= 0x00 && *(char *)ch <= 0xff)
	{
       //���㴦�� 
       TYPE_SIZE = 1;
       printf("type state\n");
	   return data_length_state;//����data length״̬ 
	}else
	   return init_state;	
}

//֡DATA LENGTH״̬
func_type data_length_state(void *ch)
{
	if (*(u8 *)ch >= 0x00 && *(u8 *)ch <= 0xff)//����жϿ���ȥ,�Ƕ����,����ʾ����u8������. 
	{
       //���㴦�� 
       TYPE_SIZE = 1;
	   length = *(char *)ch; //��¼���ݳ���,��Ϊ��һ��״̬Ҫ��
	   printf("data lenth state\n");
	   return data_state;//����data״̬ 
	}else
	   return init_state;	
}

//֡DATA״̬
func_type data_state(void *ch)
{
	if (ch != NULL)
	{
		//���㴦��
		//�����û��ص�����,��������
		//��ʱָ��ch,Ӧ��ָ�����ݶεĵ�һ���ֽڳ�,��ô������
		ip = length;
		length = 0;

		TYPE_SIZE = ip;
		data_copy(repository, ch, ip);
		printf("data state\n");
		return cs_state;//����cs״̬ 
	}else
		return init_state;	
}

//֡CS״̬,�����У�鲢δ���� 
//
func_type cs_state(void *ch)
{
	if (ch != NULL)
	{
       //���㴦�� 
       printf("cs state\n");
       TYPE_SIZE = 1;
	   return end_state;//����end״̬ 
	}else
	   return init_state;	
}

//֡End״̬ 
func_type end_state(void *ch)
{
	if (*(u16 *)ch == 0xbeef)
	{
       //���㴦�� 
       printf("end state\n");
       TYPE_SIZE = 0;
	   return NULL;//����״̬ 
	}else
	   return init_state;	
}

//״̬�������к��� 
//����Ӧ����һ��ָ�� 
//ָ����ô����
void run_state(void *ch)
{
	state = init_state;
	u16 i = 11; //�Ҳ��Ե�ʱ���õ�,��bug,������ѭ�� 
	while (state && i > 0)
	{
		printf("[%x] --- [%d]\n", (u32)ch, i);
		state = (*state)(ch);
		ch = get_next_point(ch, TYPE_SIZE);//ȡ����һ��ָ��
		i--;
	}
}

//�ƶ�ָ��,��ȡ��һ��״̬,ָ��ֵ 
void *get_next_point(void *ch, unsigned char size)
{
 	 return ch + size;
}
//���û��������ݴ��,����ŵĵط�Ҳ�����û�����,�Լ����Ĵ�С 
u8 make_pack(void *data, u8 length, u8 type, u8 *buf, u8 *len)
{
	u8 cs, i;
	u8 head[] = {0xa5, 0xa5};
	u16 end[] = {0xef, 0xbe}; 
	//buf = (u8 *)malloc(len + 7); //7��Э��֡����DATA��ĳ���,���ǹ̶���
	                               //�����������ڴ�,��Ȼ�����ڴ�й©.
 	if (buf == NULL) 
 	{
 		printf("malloc error\n");
	    exit(1);
 	}
 	//����У����,type��data length��data�����������ֽڵ����ֵ 
 	cs = head[0]^head[1]^length;
 	for (i = 0; i < length; i++)
 	{
 		cs = cs ^ *(u8*)(data + i);
 	}
 	
 	memcpy(buf, (void *)head, 2);
	memcpy(buf+2, &type, 1);
	memcpy(buf+3, &length, 1);
	memcpy(buf+4, data, length);
	memcpy(buf+4+length, &cs, 1);
	memcpy(buf+4+length+1, &end, 2);
	
	if (len != NULL)
		*len = 4+length+1+2;

	return 1;
}
int main()
{
	u8 good[] = {0xa5, 0x00, 0xa5, 0xa5, 0x22, 0x04, 0x38, 0x01, 0x03, 0x42, 0x00, 0xef, 0xbe};
	u8 arr[]={1,2,3} ;
	u8 buff[1024];
	u8 len=0;
	u8 type = 0x34;
	
	run_state((void *)good);
	
	//printf("%x\n", *(u16 *)&good[7]);
	//printf("%x\n", *(u16 *)&good[1]);
	printf("%x\n", *(u16*)&repository[1]);
	printf("%x\n", ip);
	make_pack(arr, 3, type, buff, &len);
	
	run_state((void *)buff);
	printf("[%d]\n", len);
	printf("%x\n", *(u16*)&repository[1]);
	printf("%x\n", ip);
}
