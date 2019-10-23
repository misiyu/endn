#ifndef _EDATA_H_
#define _EDATA_H_

#include "packet.h"
class EData
{
public:
	EData(const string &daddr);
	EData(const uint8_t* pkt , int pkt_len);
	~EData();

	void setContent(const uint8_t* content , int clen) ;
	void setContent(const char* content , int clen) ;
	int get_packet(char* &data) ;
	string getName() const ;
	string getContent() const ;

private:
	/* data */
	string daddr ;
	bool has_content ;
	char *content ;
	uint16_t packet_len ;
	char packet[MAX_P_SZ] ;
};

#endif 
