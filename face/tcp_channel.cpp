#include <iostream>
#include "tcp_channel.h"

using namespace std;

Tcp_Channel::Tcp_Channel(int sockfd):Channel(sockfd){
	//this->state = 1;
	//this->sockfd = sockfd ;
}
Tcp_Channel::~Tcp_Channel(){

}

void Tcp_Channel::start(){
	printf("start tcp channel >>> \n") ;
	int ret = pthread_create(&(send_td) , NULL , send ,(void*)this) ;
	    ret = pthread_create(&(recv_td) , NULL , recv ,(void*)this) ;
}
void Tcp_Channel::stop(){
	this->state = 0 ;
}

void* Tcp_Channel::recv(void *para)
{
	Tcp_Channel *_this = (Tcp_Channel*)para ;
	Tcp_RQueue *recv_queue = &(_this->mrqueue) ;
	while(_this->state){
		int free_size = 0;
		while((free_size = recv_queue->get_cfree_space()) <= 0) ;
		// buff has not free space , wait ...
		char *buffp = recv_queue->get_rear_p() ;
		int rlen = read(_this->sockfd , buffp , free_size) ;
		recv_queue->add_n(rlen) ;
		printf("recv <<< \n") ;
		//sleep(1) ;
	}
}

void* Tcp_Channel::send(void *para){
	//= (RQueue*)para ;
	Tcp_Channel *_this = (Tcp_Channel*)para ;
	Tcp_SQueue *send_queue = &(_this->msqueue) ;
	while(_this->state){
		while(send_queue->is_empty()) ;
		// wait until has data in send queue 
		int data_len = send_queue->get_data_len();
		char *buffp = send_queue->get_head_p();
		// write data to socket
		
		send_queue->rmv_n(data_len);
		printf("send >>> \n") ;
		//sleep(1) ;
	}
}
