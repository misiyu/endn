#include <iostream>
#include "ether_channel.h"

using namespace std;

Ether_Channel::Ether_Channel(string &if_name , const uint8_t * s_mac){
	// 01:00:5e:00:17:aa
	uint8_t default_dmac[] = {0x1 , 0x0 , 0x5e , 0x0 , 0x17 , 0xaa} ;
	memcpy(this->d_mac , default_dmac , 6) ;
	this->if_name = if_name ;
	memcpy(this->s_mac , s_mac , 6) ;
}
Ether_Channel::~Ether_Channel(){

}

void Ether_Channel::start(){
	printf("start ethernet channel >>> \n");
	int ret = pthread_create(&(send_td) , NULL , send , (void*)this) ;
}
void Ether_Channel::stop(){
	printf("stop ethernet channel >>> \n");
}

int get_eth_sockfd(struct sockaddr_ll &device , const uint8_t *s_mac ,
		 const char *if_name){
	int sockfd ;
	memset(&device , 0 , sizeof(device)) ;
	if((device.sll_ifindex = if_nametoindex(if_name)) == 0){
		printf("fail to obtain interface %s's index \n" , if_name) ;
		exit(1) ;
	}
	device.sll_family = AF_PACKET ;
	memcpy(device.sll_addr , s_mac , 6) ;
	device.sll_halen = htons(6) ;

	if(( sockfd = socket(PF_PACKET , SOCK_RAW , htons(NET_TYPE)) ) < 0){
		printf("can not open a sockfd , maybe you are not root\n") ;
		exit(1) ;
	}
	return sockfd ;
}

void *Ether_Channel::send(void *param){
	Ether_Channel *_this = (Ether_Channel*)param ;
	Tcp_SQueue *send_queue = &(_this->msqueue) ;

	int sockfd ;  //  
	struct sockaddr_ll device ;

	// 绑定一个网卡并获得一个sockfd 
	sockfd = get_eth_sockfd(device , _this->s_mac , _this->if_name.data()) ;

	char ether_p[MTU] ;
	memcpy(ether_p , _this->d_mac , 6) ;
	memcpy(ether_p+6 , _this->s_mac , 6) ;
	ether_p[12] = (NET_TYPE >> 8) ;
	ether_p[13] = (NET_TYPE & 0xff) ;
	while(_this->state){
		pthread_mutex_lock(&(_this->sd_mutex));	
		while(send_queue->is_empty()) {
			memcpy(ether_p , _this->d_mac , 6) ;
			pthread_cond_wait(&(_this->send_data) , &(_this->sd_mutex)) ;
		}
		pthread_mutex_unlock(&(_this->sd_mutex));	

		cout << "ether channel send 发送数据" << endl ;

		int data_len = send_queue->get_cdata_len();
		char *buffp = send_queue->get_head_p();
		int send_len = data_len ;
		if(send_len > MTU) send_len = MTU ;
		if(send_len <= 50) {
			*((uint16_t*)(ether_p+14)) = send_len ;
			memcpy(ether_p+16 , buffp , send_len) ;
			send_len += 16 ;
		}else{
			memcpy(ether_p+14 , buffp , send_len) ;
			send_len += 14 ;
		}
		// write data to socket
		// 将数据发送出去
		sendto(sockfd , ether_p , send_len , 0 ,
				(struct sockaddr*)&(device),
				sizeof(device)) ;
	}
	close(sockfd) ;
}
