#include <iostream>
#include "channel.h"

using namespace std;

Channel::Channel(){
	pthread_cond_init(&recv_data , NULL) ;  // 初始化条件变量
	pthread_mutex_init(&rd_mutex , NULL) ;
	pthread_cond_init(&send_data , NULL) ;  // 初始化条件变量
	pthread_mutex_init(&sd_mutex , NULL) ;
}


Channel::Channel(int sockfd){
	this->sockfd = sockfd ;
	Channel() ;
}

Channel::~Channel(){

}

void Channel::start(){

}

void Channel::stop(){

}

void Channel::set_sockfd(int sockfd){
	this->sockfd = sockfd ;
}
