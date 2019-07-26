#include <iostream>
#include "tcp_channel.h"

using namespace std;

Tcp_Channel::Tcp_Channel(int sockfd):Channel(sockfd){
	this->state = 1;
	this->sockfd = sockfd ;
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
		cout << "channel start recv " << endl ;
		int free_size = 0;
		while((free_size = recv_queue->get_cfree_space()) <= 0 && _this->state) {
			cout << "wait for free space " << endl ;
		}
		// buff has not free space , wait ...
		char *buffp = recv_queue->get_rear_p() ;
		int rlen = read(_this->sockfd , buffp , free_size) ;
		if(rlen <= 0) break ;
		cout << "channel recv len = " << rlen << endl ;
		bool need_signal = recv_queue->is_empty() ;
		recv_queue->add_n(rlen) ;
		if(need_signal) pthread_cond_signal(&(_this->recv_data)) ;
		printf("recv <<< \n") ;
		//sleep(1) ;
	}
}

void* Tcp_Channel::send(void *para){
	//= (RQueue*)para ;
	Tcp_Channel *_this = (Tcp_Channel*)para ;
	Tcp_SQueue *send_queue = &(_this->msqueue) ;
	while(_this->state){
		pthread_mutex_lock(&(_this->sd_mutex));	
		while(send_queue->is_empty()) {
			pthread_cond_wait(&(_this->send_data) , &(_this->sd_mutex)) ;
		}
		pthread_mutex_unlock(&(_this->sd_mutex));	

		cout << "tcp channel send 发送数据" << endl ;

		int data_len = send_queue->get_cdata_len();
		char *buffp = send_queue->get_head_p();
		// write data to socket
		// 将数据发送出去
		int wlen = write(_this->sockfd, buffp , data_len) ;
		if(wlen <= 0) break ;
		cout << "Channel send len = " << wlen << endl ;
		
		send_queue->rmv_n(data_len);
		printf("send >>> \n") ;
		//sleep(1) ;
	}
}
