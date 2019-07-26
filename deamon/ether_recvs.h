#ifndef _ETHER_RECVS_H_
#define _ETHER_RECVS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>  // memset,memcpy
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>   //struct ifreq 
#include <linux/if_ether.h>
#include <linux/if_packet.h>  // struct sockaddr_ll
#include <pthread.h>
#include <string.h>
#include <vector>

#include "flist.h"

#define NET_TYPE 0x8625
#define MAX_IF 50

class Ether_RecvS
{
public:
	Ether_RecvS();
	~Ether_RecvS();
	void start() ;
	void stop() ;
	void get_ifs() ;
	static void *recv(void *param) ;

private:
	/* data */
	pthread_t m_tid ;
	int if_fs[MAX_IF] ;
	FList *m_flist ;
};

#endif 
