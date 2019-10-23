#ifndef _EINTEREST_H_
#define _EINTEREST_H_
#include <iostream>
#include <string.h>

#include "packet.h"
using std::string ;

class EInterest
{
public:
	// 构造一个兴趣包，应传入兴趣包名，以及源地址
	EInterest(string &daddr , string &saddr)	;
	EInterest(const uint8_t *pkt , int pkt_len)	;
	~EInterest();
	void setInterestLifetime();
	void setContent(const uint8_t* content , int clen) ;
	void setContent(const char* content , int clen) ;
	int get_packet(char *&data) ;
	string getName() const ;
	string get_saddr() const ;
	string getContent() const ;

private:
	/* data */
	string daddr ;
	bool has_content ;
	char *content ;
	uint16_t packet_len ;
	char packet[MAX_P_SZ] ;
	string saddr ;
	
};

#endif 
