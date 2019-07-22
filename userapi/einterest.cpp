#include <iostream>
#include "einterest.h"

using namespace std;


EInterest::EInterest(EName &ename , string &source_id): ename(ename),
	msource_name(source_id){
	this->has_content = false ;
	this->packet_len = 3 ;
	packet[0] = 0x5 ;   // 请求包标识 
	uint16_t ename_len =  enpacket(&packet[3],ename.getData() , 0x7) ;
	packet_len += ename_len ;
	uint16_t s_addr_len = enpacket(&packet[packet_len] , source_id , 0x7);
	packet_len += s_addr_len ;
	packet[packet_len] = 0x9 ;   // 其它信息标识
	*((uint16_t*)&packet[packet_len+1]) = 0x1 ;
	packet[packet_len+3] = 0xff ;     // TTL 
	packet_len += 4 ;
	*((uint16_t*)&packet[1]) = packet_len-3 ;
}
EInterest::~EInterest() {
	free(packet) ;
}

string EInterest::getName(){
	return ename.getData() ;
}

void EInterest::setContent(const uint8_t* content , int c_len){
	if(c_len > 0xffff) return ;
	uint16_t mclen = c_len ;
	packet[packet_len++] = 0x8 ;
	*((uint16_t*)&packet[packet_len]) = mclen ;
	packet_len += 2 ;
	memcpy(&packet[packet_len], content ,mclen ) ;
	packet_len += mclen ;
	*((uint16_t*)&packet[1]) = packet_len-3 ;
}

void EInterest::setInterestLifetime(){

}

int EInterest::get_packet(char* &data){
	data = (char*)malloc(packet_len);
	memcpy(data,packet,packet_len) ;
	return this->packet_len ;
}
