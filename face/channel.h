// create by wgh
#ifndef _CHANNEL_H_
#define _CHANNEL_H_
#include "tcp_rqueue.h"  
#include "tcp_squeue.h"  

class Channel
{
public:
	Channel() ;
	Channel(int sockfd) ;
	virtual ~Channel() ;
	virtual void start() ;
	virtual void stop() ;
	virtual int get_state() ;
	virtual void set_dmac(const uint8_t *d_mac) ;

	Tcp_RQueue mrqueue ;
	Tcp_SQueue msqueue ;
protected:
	int sockfd ;
	int state ;
	pthread_t send_td ;
	pthread_t recv_td ;
};

#endif 
