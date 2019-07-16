#ifndef _CHANNEL_H_
#define _CHANNEL_H_
#include "tcp_rqueue.h"  
#include "tcp_squeue.h"  

class Channel
{
public:
	Channel(int sockfd){
		this->sockfd = sockfd ;
	}
	virtual ~Channel(){

	}
	int send(){

	}
	int recv(){

	}
	virtual void start(){

	}
	virtual void stop(){

	}
	Tcp_RQueue mrqueue ;
	Tcp_SQueue msqueue ;
protected:
	int state ;
	int sockfd ;
	pthread_t send_td ;
	pthread_t recv_td ;
private:
	/* data */
};

#endif 
