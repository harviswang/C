#include <stdio.h>
#include "protocol.h"

int socket_msg_cpy_in(socket_cache *cache, unsigned char *buf, int len)
{
    int n = 0;
    
    while (n < len)
    {
        cache->buf[n] = buf[n];
        n++;
    }
    
    return n;
}
socket_msg_cpy_out(cache, cache->recv_msg.data, cache->current, cache->recv_msg.len);
//关键实现
//1、把接收到的数据存储到缓冲中，并准备解析
//copy the unparsed data to cache, and parsed them
int socket_msg_pre_parse(
                            int fd, 
                            socket_cache *cache,
                            unsigned char *buf, 
                            int len, 
                            void *args)
{
	int n = 0;
	unsigned char *p = buf;
	//when reading buffer's length is greater than cache's left length,
	//we should copy many times.
	cache->args = args;
	while(1){
		n = socket_msg_cpy_in(cache, p, len);
		if(n == 0){
			return FALSE;//cache is full	
		}
		//parse and handle socket message from cache
		socket_msg_parse(fd, cache);
		
		if(n == len){
			return TRUE; //copy completed
		}
		//move the pointer
		p 	= p + n;
		len = len - n;
	}
	
	return TRUE;
		
}
//2、递归解析消息：
//parsed the packaged data, and invoke callback function
void socket_msg_parse(int fd, socket_cache *cache)
{
	int current_len;
	int p, q;
	int i;
	int find;
	
	if(cache->front == cache->rear && cache->tag == 0){
		//D("socket cache is empty!\n");
		return;	
	}
	
	//calculate the current length of cache
	if(cache->current >= cache->front){
		current_len = cache->len - (cache->current - cache->front);
	}
	else{
		current_len = cache->rear - cache->current;	
	}
	
	switch(cache->strategy){
		case SEARCH_HEAD://to find a Head format in cache
			if(current_len < SOCKET_MSG_HEAD_SIZE){
				return;	
			}
			find = FALSE;
			for(i = 0; i < current_len - 1; i++){
				p = cache->current;
				q = (cache->current + 1) % SOCKET_MSG_CACHE_SIZE;
				if(	(cache->buf[p] == (SOCKET_MSG_HEAD >> 8))&&
					(cache->buf[q] == (SOCKET_MSG_HEAD & 0xff))){
					
					find = TRUE;
					break; //exit for loop
				}
				else{
					//current pointer move to next
					cache->current = q;
					//delete one item
					cache->front = cache->current;
					cache->len --;
					cache->tag = 0;
				}
			}
			
			if(find == TRUE){	
				//move 2 items towards next
				cache->current = (cache->current + 2) % SOCKET_MSG_CACHE_SIZE;	
				//we found the head format, go on to find Type byte
				cache->strategy = SEARCH_TYPE;		
			}
			else{
				//if there is no head format ,delete previouse items
				LOGE("socket message without head: %x!\n",SOCKET_MSG_HEAD);
				//go on to find Head format
				cache->strategy = SEARCH_HEAD;	
			}
			break;
			
		case SEARCH_TYPE://to find the type byte in cache
			if(current_len < SOCKET_MSG_TYPE_SIZE){
				return ;	
			}
			//get the value of type
			//cache->type = cache->buf[cache->current];
			cache->recv_msg.type = cache->buf[cache->current];
			cache->current = (cache->current + 1) % SOCKET_MSG_CACHE_SIZE;
			//we found Type byte, go on to find Datalen format
			cache->strategy = SEARCH_LEN;
			break;
			
		case SEARCH_LEN://to find the datalen byte in cache
			if(current_len < SOCKET_MSG_LEN_SIZE){
				return ;	
			}
			if(cache->buf[cache->current] > SOCKET_MSG_DATA_SIZE){
				LOGE("the data len of message out of size: %d!\n",SOCKET_MSG_DATA_SIZE);
				//delete the frist item 'a5'
				//move back 2 items
				cache->current = cache->current >= 2 ? (cache->current - 2) : (SOCKET_MSG_CACHE_SIZE - 2 + cache->current);
				cache->front = cache->current;
				//length sub 2
				cache->len -= 2;
				cache->tag = 0;
				//go on to find Head format
				cache->strategy = SEARCH_HEAD;
			}
			else{
				//get the value of datalen
				//cache->data_len = cache->buf[cache->current];
				cache->recv_msg.len = cache->buf[cache->current];
				cache->current = (cache->current + 1) % SOCKET_MSG_CACHE_SIZE;
				//we found datalen byte, go on to find End format
				cache->strategy = SEARCH_END;
			}
			break;
			
		
			
		case SEARCH_END:
			if(current_len < (cache->recv_msg.len + SOCKET_MSG_END_SIZE)){
				return;	
			}
			//because we have known the data bytes' len, so we move the very  
			//distance of datalen to see if there is End format. 
			p = (cache->current + cache->recv_msg.len) % SOCKET_MSG_CACHE_SIZE; 
			q = (cache->current + cache->recv_msg.len + 1) % SOCKET_MSG_CACHE_SIZE; 
			if(	(cache->buf[p] == (SOCKET_MSG_END >> 8))&&
				(cache->buf[q] == (SOCKET_MSG_END & 0xff)) ){
				socket_msg_cpy_out(cache, cache->recv_msg.data, cache->current, cache->recv_msg.len);
				if(cache->handle != NULL){
					//cache->handle(fd, cache->buf + cache->data_index, cache->data_len);
					cache->handle(fd, &cache->recv_msg, cache->args);	
				}
				//delete all previous items
				cache->current = (q + 1) % SOCKET_MSG_CACHE_SIZE;
				cache->front = cache->current;
				cache->len -= (cache->recv_msg.len + SOCKET_MSG_FORMAT_SIZE);
				cache->tag =0;
				
			}
			else{
				LOGE("socket message without end: %x!\n",SOCKET_MSG_END);
				//delete the frist item 'a5'
				//move back 3 items
				cache->current = cache->current >= 3 ? (cache->current - 3) : (SOCKET_MSG_CACHE_SIZE - 3 + cache->current);
				cache->front = cache->current;
				//length sub 3
				cache->len -= 3;
				cache->tag = 0;
					
			}
			//go on to find Head format
			cache->strategy = SEARCH_HEAD;
			break;
			
		default:
				break;
				
				
	}
	
	//parse new socket message
	socket_msg_parse(fd,cache);
}

