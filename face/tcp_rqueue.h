// create by wgh
#ifndef _TCP_RQUEUE_H_
#define _TCP_RQUEUE_H_

#include "common/r_queue.h"

class Tcp_RQueue : public R_Queue{
public:
	bool has_packet() ;
	int rmv_pack(){
		int16_t pack_len = 0 ;
		memcpy(&pack_len,&buff[head+1],sizeof(int16_t));
		// get the length of a endn packet
		head = (head + 3 + pack_len)%QUEUE_SZ ;
	}


	Tcp_RQueue(){
	}
	virtual ~Tcp_RQueue(){

	}

private:

};

#endif 
