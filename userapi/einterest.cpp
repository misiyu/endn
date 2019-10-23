#include <iostream>
#include "einterest.h"

using namespace std;


EInterest::EInterest(string &daddr , string &source_id): 
	daddr(daddr),
	saddr(source_id){
	this->has_content = false ;
	this->content = NULL ;
	this->packet_len = 3 ;
	packet[0] = 0x5 ;   // 请求包标识 
	uint16_t ename_len =  enpacket(&packet[3],daddr , 0x7) ;
	packet_len += ename_len ;
	uint16_t s_addr_len = enpacket(&packet[packet_len] , source_id , 0x7);
	packet_len += s_addr_len ;
	packet[packet_len] = 0x9 ;   // forwarding hint
	*((uint16_t*)&packet[packet_len+1]) = 0x1 ;
	packet[packet_len+3] = 0xff ;     // TTL = 127 
	packet_len += 4 ;
	*((uint16_t*)&packet[1]) = packet_len-3 ;
}

EInterest::EInterest(const uint8_t *pkt, int pkt_len):
	packet_len(pkt_len)
{
	memcpy(packet , pkt, pkt_len) ;
	daddr = decode_name((uint8_t*)(packet+3)) ;
	uint16_t daddr_len = *((uint16_t*)(packet+4)) ;
	saddr = decode_name((uint8_t*)(packet+6+daddr_len)) ;
	uint16_t saddr_len = *((uint16_t*)(packet+7+daddr_len)) ;
	//mclen = *((uint16_t*)(packet+10+daddr_len+saddr_len)) ;
	this->content = &packet[12+daddr_len+saddr_len] ;
}

EInterest::~EInterest() {
}

string EInterest::getName() const {
	return this->daddr ;
}

string EInterest::get_saddr() const {
	return this->saddr ;
}

string EInterest::getContent() const {
	if(content == NULL) return "" ;
	uint16_t c_len = *((uint16_t*)(content-2)) ;
	string ret(content,c_len) ;
	return  ret;
}

void EInterest::setContent(const uint8_t* content , int c_len){
	if(c_len > 0xffff || c_len <= 0) return ;
	uint16_t mclen = c_len ;

	uint8_t fhint[4] ;
	packet_len-=4 ;
	memcpy(fhint , &packet[packet_len],4) ;

	packet[packet_len++] = 0x8 ;
	*((uint16_t*)&packet[packet_len]) = mclen ;
	packet_len += 2 ;
	this->content = &packet[packet_len] ;
	memcpy(&packet[packet_len], content ,mclen ) ;
	packet_len += mclen ;

	memcpy(&packet[packet_len],fhint,4) ;
	packet_len += 4 ;
	*((uint16_t*)&packet[1]) = packet_len-3 ;
}
void EInterest::setContent(const char* content , int c_len){
	setContent((uint8_t*)content , c_len) ;
}

void EInterest::setInterestLifetime(){

}

int EInterest::get_packet(char* &data){
	data = (char*)malloc(packet_len);
	memcpy(data,packet,packet_len) ;
	return this->packet_len ;
}
