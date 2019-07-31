#include <iostream>
#include "tcp_channel.h"

using namespace std;

Tcp_Channel::Tcp_Channel(int sockfd):Channel(sockfd){
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
	static int recv_count = 0 ;
	Tcp_Channel *_this = (Tcp_Channel*)para ;
	Tcp_RQueue *recv_queue = &(_this->mrqueue) ;
	//_this->state = 1; 
	//cout << "Tcp_Channel recv thread state = " << _this->state << endl ;
	//while(1){
		//char buff[50000] ;
		//int rlen = read(_this->sockfd , buff , 50000) ;
		//if(rlen <= 0) break ;
		//cout<<" <<<<<<<<<<<<<<<<<<<<< tcp channel recv len = " << rlen << endl ;
		//recv_count += rlen ;
		//cout << " <<<<<< tcp channel total recv len = " << recv_count << endl ;
	//}
	//exit(1) ;

	while(_this->state){
		//cout << "channel start recv " << endl ;
		int free_size = 0;
		while((free_size = recv_queue->get_cfree_space()) <= 0 && _this->state) {
			cout << "wait for free space " << endl ;
		}
		char *buffp = recv_queue->get_rear_p() ;
		cout<<"Tcp_Channel rqueue data len = "<<recv_queue->get_data_len()<<endl ;
		cout << "Tcp_Channel recv free size = " << free_size << endl ;
		int rlen = read(_this->sockfd , buffp , free_size) ;
		if(rlen <= 0) break ;
		cout << " <<<<<<<<<<<<<<<<<<<<< tcp channel recv len = " << rlen << endl ;
		recv_count += rlen ;
		cout << " <<<<<< tcp channel total recv len = " << recv_count << endl ;
		bool need_signal = false ;
		pthread_mutex_lock(&(_this->rd_mutex)) ;
		need_signal = recv_queue->is_empty() ;
		recv_queue->add_n(rlen) ;
		pthread_mutex_unlock(&(_this->rd_mutex)) ;
		if(need_signal) pthread_cond_signal(&(_this->recv_data)) ;
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
