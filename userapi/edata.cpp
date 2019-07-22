#include <iostream>
#include "edata.h"

using namespace std;

EData::~EData(){

}
EData::EData(const string &daddr){
	this->daddr = daddr ;
	this->has_content = false ;
	this->packet_len = 3 ;
	packet[0] = 0x6 ;   // 请求包标识 
	uint16_t daddr_len = enpacket(&packet[3],daddr, 0x7);
	packet_len += daddr_len ;
}

void EData::setContent(const uint8_t* content , int c_len){
	if(c_len > 0xffff) return ;
	uint16_t mclen = c_len ;
	packet[packet_len++] = 0x8 ;
	*((uint16_t*)&packet[packet_len]) = mclen ;
	packet_len += 2 ;
	memcpy(&packet[packet_len], content ,mclen ) ;
	packet_len += mclen ;
	*((uint16_t*)&packet[1]) = packet_len ;
}

int EData::get_packet(char *data){
	data = (char*)malloc(packet_len);
	memcpy(data,packet,packet_len) ;
	return this->packet_len ;
}
