#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#define SOCKET_MSG_SIZE 1024
#define SOCKET_MSG_CACHE_SIZE 1024*16
#define SOCKET_MSG_HEAD_SIZE 2
#define SOCKET_MSG_HEAD 0xa5a5
#define SOCKET_MSG_TYPE_SIZE 1
#define SOCKET_MSG_LEN_SIZE 1
#define SOCKET_MSG_DATA_SIZE 255
#define SOCKET_MSG_END_SIZE 2
#define SOCKET_MSG_END 0xbeef
#define SOCKET_MSG_FORMAT_SIZE 2
#define LOGE(x, y) printf(x,y)

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
//���Ҳ���ö�٣����ڲ���ʱ�жϲ���֡�ṹ���ĸ���λ
typedef enum{
	SEARCH_HEAD,
	SEARCH_TYPE,
	SEARCH_LEN,
	//SEARCH_CS,
	SEARCH_END,
	SEARCH_NONE
}cache_strategy;
//��Ϣ�ṹ�壬���ڴ洢�ӻ����н�����������
typedef struct{
	unsigned char data[SOCKET_MSG_SIZE];			//data
	int len;
	unsigned char type;
}socket_msg;
//�ص����������ڴӻ����н�������Ϣʱ����
typedef void (*tp_socket_msg_handle)(int fd, socket_msg *msg,void *args);
//ѭ�����У����ڻ�����յ�������
typedef struct{
	unsigned char buf[SOCKET_MSG_CACHE_SIZE]; //buffer for storing data read from client
	int front;
	int rear;
	int current;	
	int len;
	int tag;			//mark that whether the cache is full,1-full,0-not full
	cache_strategy strategy;
	tp_socket_msg_handle handle;//callback function to invoke when a message is parsed out		
	void* args;									//external parameter
	socket_msg recv_msg;							//parsed message
	
}socket_cache;
//http://blog.csdn.net/xnwyd/article/details/7668742?reload
#endif

