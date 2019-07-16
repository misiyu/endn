#ifndef _ETHER_CHANNEL_H_
#define _ETHER_CHANNEL_H_

#include "channel.h"

class Ether_Channel: public Channel
{
public:
	Ether_Channel(uint8_t local_mac_p[] , int8_t remote_mac_p[] , int mtu_p);
	virtual ~Ether_Channel();

private:
	/* data */
	uint8_t local_mac[6] ;
	uint8_t remote_mac[6] ;
	int mtu ;
};

#endif 
