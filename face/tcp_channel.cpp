#include <iostream>
#include "tcp_channel.h"

using namespace std;

Tcp_Channel::Tcp_Channel(int sockfd):Channel(sockfd){
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

// 功能： 接收数据进程,将数据接收到接收队列中
void* Tcp_Channel::recv(void *para)
{
	static int recv_count = 0 ;
	Tcp_Channel *_this = (Tcp_Channel*)para ;
	Tcp_RQueue *recv_queue = &(_this->mrqueue) ;

	while(_this->state){
		int free_size = 0;
		recv_queue->wait4space() ;
		free_size = recv_queue->get_cfree_space() ;
		char *buffp = recv_queue->get_rear_p() ;
		int rlen = read(_this->sockfd , buffp , free_size) ;
		if(rlen <= 0){
			cout << "face close" << endl ;
			_this->state = 0 ;
			_this->msqueue.add_n(1) ;
			_this->mrqueue.add_n(1) ;
			break ;
		}
		//cout << " <<<<<<<<<<<<<<<<<< tcp channel recv len = " << rlen << endl ;
		recv_count += rlen ;
		//cout << " <<<<<< tcp channel total recv len = " << recv_count << endl ;
		bool need_signal = false ;
		recv_queue->add_n(rlen) ;
	}
}

// 功能： 数据发送进程，将发送队列中的数据发送出去
void* Tcp_Channel::send(void *para){
	Tcp_Channel *_this = (Tcp_Channel*)para ;
	Tcp_SQueue *send_queue = &(_this->msqueue) ;

	while(_this->state){
		send_queue->wait4data() ;
		int data_len = send_queue->get_cdata_len();
		char *buffp = send_queue->get_head_p();
		// write data to socket
		// 将数据发送出去
		int wlen = write(_this->sockfd, buffp , data_len) ;
		if(wlen <= 0) break ;
		send_queue->rmv_n(data_len);
		printf("send >>> \n") ;
	}
}
