//���������͵Ĵ�С,ͨ��ָ�봫��������
//�е��ź�,���ְ취û����ARM����Ҫ�ڴ����ĳ���ʹ�� 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//�ڷ��صĺ�����֮ǰ,�Ӹ�ǿ������ת��,������������, 
//Э����ص�����,ע��ȫ�����޷�������,�з������׳��鷳.��ͬ�ı�����,���ܻ���ͬ. 
typedef unsigned int u32;
typedef unsigned short int u16;
typedef unsigned char u8;

//����ָ������ָ������. 
typedef u32 (*protocol_func)(void *, u8 *);
typedef protocol_func (*func_type)(void *, u8 *);

func_type init_state(void *ch, u8 *type_size);
func_type head_state(void *ch, u8 *type_size);
func_type type_state(void *ch, u8 *type_size);
func_type data_length_state(void *ch, u8 *type_size);
func_type data_state(void *ch, u8 *type_size);
func_type cs_state(void *ch, u8 *type_size);
func_type end_state(void *ch, u8 *type_size);

//�û��ص�����
//��Ž�����������������,�Լ����ݵĳ���
void data_copy(void *dest, void *src, int len); //�򵥵Ľ����ݸ�����
unsigned char repository[256];//�ֿ�,�û��ص����������Ľ��,�洢�������� 
unsigned char length = 0, ip = 0;//length��״̬ת���õ�, ip�ǲֿ������ 
//unsigned char TYPE_SIZE = 0; //�����¼״̬��Ӧ�����͵Ĵ�С,����ط�����Ĳ����,û�뵽���õķ���. 

//�û��Զ����,����������������,�൱��һ���ص����� 
void data_copy(void *dest, void *src, int len)
{
	memcpy(dest, src, len);
	//�������Ĵ���.... 
}

//״̬����ʼ״̬ 
func_type init_state(void *ch, u8 *type_size)
{
	if (ch == NULL)
       return NULL;
	else{
		*type_size = 0;
		printf("init state\n");
		return (func_type)head_state;	
	}
	   
}

//֡HEAD״̬
//cp��ʾɨ��ָ��ĵ�ǰλ��
//type_size��ʾ��״̬�ֶ���֡�ṹ�е��ֽ��� 
func_type head_state(void *ch, u8 *type_size)
{
	if (*(u16 *)ch == 0xa5a5)
	{
       	//���㴦�� 
       	*type_size = 2;
		printf("head state\n");
	   	return (func_type)type_state;//����data length״̬ 
	}else
	{
		*type_size = 1; //ȥ��һ���ֽ�,���֡ͷ��ʾ�� 
		return (func_type)init_state;
	}
	   	
}

//֡TYPE״̬
func_type type_state(void *ch, u8 *type_size)
{
	if (*(u8 *)ch >= 0x00 && *(char *)ch <= 0xff)
	{
       //���㴦�� 
       *type_size = 1;
       printf("type state\n");
	   return (func_type)data_length_state;//����data length״̬ 
	}else
	   return (func_type)init_state;	
}

//֡DATA LENGTH״̬
func_type data_length_state(void *ch, u8 *type_size)
{
	if (*(u8 *)ch >= 0x00 && *(u8 *)ch <= 0xff)//����жϿ���ȥ,�Ƕ����,����ʾ����u8������. 
	{
       //���㴦�� 
       *type_size = 1;
	   length = *(char *)ch; //��¼���ݳ���,��Ϊ��һ��״̬Ҫ��
	   printf("data lenth state\n");
	   return (func_type)data_state;//����data״̬ 
	}else
	   return (func_type)init_state;	
}

//֡DATA״̬
//*type_size��ʾ���ݶεĳ��� 
func_type data_state(void *ch, u8 *type_size)
{
	if (ch != NULL)
	{
		//���㴦��
		//�����û��ص�����,��������
		//��ʱָ��ch,Ӧ��ָ�����ݶεĵ�һ���ֽڳ�,��ô������
		ip = length;
		length = 0;

		*type_size = ip;
		data_copy(repository, ch, ip);
		printf("data state\n");
		return (func_type)cs_state;//����cs״̬ 
	}else
		return (func_type)init_state;	
}

//֡CS״̬,�����У�鲢δ���� 
//
func_type cs_state(void *ch, u8 *type_size)
{
	if (ch != NULL)
	{
       //���㴦�� 
       printf("cs state\n");
       *type_size = 1;
	   return (func_type)end_state;//����end״̬ 
	}else
	   return (func_type)init_state;	
}

//֡End״̬ 
func_type end_state(void *ch, u8 *type_size)
{
	if (*(u16 *)ch == 0xbeef)
	{
       //���㴦�� 
       printf("end state\n");
       *type_size = 2;
	   return NULL;//����״̬ 
	}else
	   return (func_type)init_state;	
}

//״̬�������к��� 
//����Ӧ����һ��ָ�� 
//ָ����ô����
void run_state(void *ch, u16 len)
{
	func_type state;
	state = (func_type)init_state;
	u16 i = len;// = 11; //�Ҳ��Ե�ʱ���õ�,��bug,������ѭ�� 
	            //������������ݵĳ���,��������,������ĸ�ʽ��������ѭ�� 
	u8 type_size = 0;
	while (state && i > 0)
	{
		printf("[%x] --- [%d]\n", (u32)ch, i);
		state = (*state)(ch, &type_size);
		ch += type_size;//ȡ����һ��ָ��
		i--;
	}
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
 	//������װ��֡��ʽ�涨�˵���ʽ 
 	memcpy(buf, (void *)head, 2);
	memcpy(buf+2, &type, 1);
	memcpy(buf+3, &length, 1);
	memcpy(buf+4, data, length);
	memcpy(buf+4+length, &cs, 1);
	memcpy(buf+4+length+1, &end, 2);
	
	if (len != NULL)
		*len = 4+length+1+2;
	else
		return 0;
	return 1;
}
int main()
{
	u8 good[] = {0xa5, 0x00, 0xa5, 0xa5, 0x22, 0x04, 0x38, 0x01, 0x03, 0x42, 0x00, 0xef, 0xbe};
	u8 arr[]={1,2,3} ;
	u8 buff[1024];
	u8 len=0;
	u8 type = 0x34;
	
	run_state((void *)good, sizeof(good)/sizeof(good[0]));
	
	//printf("%x\n", *(u16 *)&good[7]);
	//printf("%x\n", *(u16 *)&good[1]);
	printf("%x\n", *(u16*)&repository[1]);
	printf("%x\n", ip);
	make_pack(arr, 3, type, buff, &len);
	
	run_state((void *)buff, len);
	printf("[%d]\n", len);
	printf("%x\n", *(u16*)&repository[0]);
	printf("%x\n", ip);
}
