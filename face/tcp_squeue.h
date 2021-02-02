// create by wgh
#ifndef _TCP_SQUEUE_H_
#define _TCP_SQUEUE_H_

#include "common/r_queue.h"

class Tcp_SQueue: public R_Queue
{
public :
	bool has_packet() ;
	int rmv_pack(){
		int16_t pack_len = 0 ;
		memcpy(&pack_len,&buff[head+1],sizeof(int16_t));
		// get the length of a endn packet
		head = (head + 3 + pack_len)%QUEUE_SZ ;
	}
	int add_n(int n){
		rear = (rear + n) % QUEUE_SZ ;
	}
	int rmv_n(int n){
		head = (head + n) % QUEUE_SZ ;
	}
	// get a continuous free space in the ring queue
	int get_free_space(){
		int space = 0 ;
		if(head < rear ) {
			space = QUEUE_SZ - rear ;
			if(head == 0) space - 1 ;
		}else if(head == rear){
			return 0 ;
		}else{
			space = head - rear - 1 ;
		}
		return space ;
	}
	

	Tcp_SQueue(){
		this->head = 0 ;
		this->rear = 0 ;
	}
	virtual ~Tcp_SQueue(){
	}


};

#endif 
