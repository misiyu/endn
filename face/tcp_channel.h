#ifndef _TCP_CHANNEL_H_
#define _TCP_CHANNEL_H_

#include <stdlib.h>  
#include <stdio.h>  
#include <errno.h>  
#include <string.h>  
#include <unistd.h>  
#include <netdb.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/types.h>  
#include <arpa/inet.h>  
#include <fcntl.h>  
#include <pthread.h>

#include "channel.h"  


class Tcp_Channel : public Channel
{
public:
	Tcp_Channel(int sockfd) ;
	~Tcp_Channel();
	// 收数据进程 
	static void *recv(void *para) ; 
	static void *send(void *para) ;
	void start();
	void stop();

private:
	/* data */
};



#endif 
