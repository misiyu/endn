#ifndef _EDATA_H_
#define _EDATA_H_

#include "ename.h"
#include "packet.h"
class EData
{
public:
	EData(const string &daddr);
	~EData();

	void setContent(const uint8_t* content , int clen) ;
	int get_packet(char *data) ;

private:
	/* data */
	string daddr ;
	bool has_content ;
	uint16_t packet_len ;
	char packet[MAX_P_SZ] ;
};

#endif 
