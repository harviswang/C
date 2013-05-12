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
//查找策略枚举，用于查找时判断查找帧结构的哪个部位
typedef enum{
	SEARCH_HEAD,
	SEARCH_TYPE,
	SEARCH_LEN,
	//SEARCH_CS,
	SEARCH_END,
	SEARCH_NONE
}cache_strategy;
//消息结构体，用于存储从缓存中解析出的数据
typedef struct{
	unsigned char data[SOCKET_MSG_SIZE];			//data
	int len;
	unsigned char type;
}socket_msg;
//回调函数，用于从缓存中解析出消息时调用
typedef void (*tp_socket_msg_handle)(int fd, socket_msg *msg,void *args);
//循环队列，用于缓存接收到的数据
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

