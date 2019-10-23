#ifndef _ETHER_CHANNEL_H_
#define _ETHER_CHANNEL_H_

#include <iostream>
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
#include "channel.h"

#define MTU 1500
#define NET_TYPE 0x8625
using std::string ;

class Ether_Channel: public Channel
{
public:
	Ether_Channel(string &if_name , const uint8_t *s_mac);
	virtual ~Ether_Channel();
	virtual void start() ;
	virtual void stop() ;

	void set_dmac(const uint8_t *dmac) ;
	static void *send(void *param) ;

private:
	/* data */
	uint8_t s_mac[6] ;
	uint8_t d_mac[6] ;
	string if_name ;
	char ether_p[MTU+14] ;

};

#endif 
