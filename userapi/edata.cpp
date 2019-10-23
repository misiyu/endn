#include <iostream>
#include "edata.h"

using namespace std;

EData::~EData(){

}
EData::EData(const string &daddr){
	this->daddr = daddr ;
	this->has_content = false ;
	this->content = NULL ;
	this->packet_len = 3 ;
	packet[0] = 0x6 ;   // 请求包标识 
	uint16_t daddr_len = enpacket(&packet[3],daddr, 0x7);
	packet_len += daddr_len ;
	packet[packet_len] = 0x9 ;   // 其它信息标识
	*((uint16_t*)&packet[packet_len+1]) = 0x1 ;
	packet[packet_len+3] = 0xff ;     // TTL 
	packet_len += 4 ;
	*((uint16_t*)&packet[1]) = packet_len-3 ;
}

EData::EData(const uint8_t* pkt , int pkt_len):
	packet_len(pkt_len)
{
	memcpy(packet,pkt,pkt_len) ;
	daddr = decode_name((uint8_t*)(packet+3)) ;
	uint16_t daddr_len = *((uint16_t*)(packet+4)) ;
	this->content = &packet[9+daddr_len] ;
}

// 
// 功能： 装入应答包载荷
// 参数：
//		content 数据
//		c_len 数据长度
// 修改： packet 
void EData::setContent(const uint8_t* content , int c_len){
	if(c_len > 0xffff || c_len <= 0){
		printf("error content set into data packet is too long\n") ;
		return ;
	}
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
void EData::setContent(const char* content , int c_len){
	setContent((uint8_t*)content , c_len) ;
}

// 功能 ： 获取packet 成员中的数据
// 参数 ： 
//		data 用来保存packet中数据的buff, 注意使用引用或双重指针
// 返回值：  int 数据包长度

int EData::get_packet(char* &data){
	data = (char*)malloc(packet_len);
	memcpy(data,packet,packet_len) ;
	return this->packet_len ;
}

string EData::getName() const{
	return this->daddr ;
}

string EData::getContent() const {
	if(content == NULL) return "" ;
	uint16_t c_len = *((uint16_t*)(content-2)) ;
	string ret(content,c_len) ;
	return  ret;
}
