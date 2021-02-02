// create by wgh
#ifndef _PACKET_H_
#define _PACKET_H_

#include <string.h>

using std::string ;
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

static uint16_t encode_name(uint8_t *buff , const string &name) {
	uint16_t len = 3 ;
	int name_len = name.length();
	buff[0] = 0x7 ;   // 请求包名称标识
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
static string decode_name(const uint8_t *name ) {
	string result = "" ;
	uint16_t name_len = *((uint16_t*)(name+1))+3 ;
	uint16_t s_len = 3 ;
	while(s_len < name_len){
		uint16_t c_len = *((uint16_t*)(name+s_len+1)) ;
		string comp((char*)( name+s_len+3 ),c_len) ;
		s_len += (3+c_len) ;
		result += ("/"+comp) ;
	}
	return result ;
}

#endif 
