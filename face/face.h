#ifndef _FACE_H_
#define _FACE_H_
#include <iostream>
#include <vector>
#include "tcp_channel.h"
#include "ether_channel.h"
#include "common/globle_var.h"
#include "table/fib.h"
#include "table/pit.h"

#define RQueue (_this->mchannel->mrqueue)

using std::string ;
using std::vector ;

enum CH_TYPE { ETH , TCP } ;
enum FACE_STATE { ACTIVE , DEAD } ;

class FList ;
class Face
{
public:
	Face(const char *dip , int sockfd , int face_id);
	Face(string &if_name , const uint8_t *s_mac , int face_id);
	void stop();
	void start() ;
	static void *forward(void *param) ;
	int send2face(vector<int> &face_list, int clen) ;

	// 功能： 往发送队列里加入长度为len的数据
	// 参数： 
	//		data 数据
	//		len 数据长度
	//	
	int add2chsq(char *data , int len) ;

	// 功能： 往接收队列里加入长度为len的数据
	// 参数： 
	//		data 数据
	//		len 数据长度
	//	
	int add2chrq(char *data , int len) ;

	enum FACE_STATE get_state(){
		return this->m_state ;
	}
	~Face() ;
	void set_dmac(const uint8_t *dmac) ;

	enum FACE_STATE m_state ;
	string daddr ;
private:
	/* data */
	int face_id ;
	pthread_t mtid ;
	enum CH_TYPE m_ch_type ;
	Channel * mchannel ;
	FIB *m_fib ;
	PIT *m_pit ;
	FList *m_flist ;
	
};

#endif 
