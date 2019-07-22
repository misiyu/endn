#ifndef _PACKET_H_
#define _PACKET_H_

#include <string.h>

#define MAX_P_SZ 8400
static uint16_t enpacket(char *buff , const string &name, uint8_t type){
	uint16_t len = 3 ;
	int name_len = name.length();
	buff[0] = type ;   // 请求包名称标识
	int i ;
	int c_len_i = -1 ;
	uint16_t c_len = 0 ;
	for (i = 0; i < name_len; i++) {
		buff[len++] = name[i];
		c_len ++ ;
		if(name[i] == '/'){  
			c_len -- ;
			if(c_len_i != -1) *((uint16_t*)&buff[c_len_i]) = c_len ;
			c_len_i = len ;
			c_len = 0 ;
			len += 2 ;
		}
	}
	*((uint16_t*)&buff[c_len_i]) = c_len ;
	*((uint16_t*)&buff[1]) = len-3 ;
	return len ;
}

struct packet_t{
	char *data;
	uint16_t len ;
	packet_t(){
		data = NULL ;
		len = 0;
	}
	~packet_t(){
		if(data != NULL) free(data) ;
	}
};

#endif 
