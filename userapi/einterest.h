#ifndef _EINTEREST_H_
#define _EINTEREST_H_
#include <iostream>
#include <string.h>

#include "ename.h"
#include "packet.h"
using std::string ;

class EInterest
{
public:
	// 构造一个兴趣包，应传入兴趣包名，以及源地址
	EInterest(EName &ename , string &source_id)	;
	~EInterest();
	void setInterestLifetime();
	string getName() ;
	void setContent(const uint8_t* content , int clen) ;
	int get_packet(char *&data) ;

private:
	/* data */
	EName &ename ;
	bool has_content ;
	uint16_t packet_len ;
	char packet[MAX_P_SZ] ;
	string &msource_name ;
	
};

#endif 
