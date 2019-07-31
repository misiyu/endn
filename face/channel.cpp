#include <iostream>
#include "channel.h"

using namespace std;

Channel::Channel(){
	this->state = 1;
	//pthread_cond_init(&recv_data , NULL) ;  // 初始化条件变量
	//pthread_mutex_init(&rd_mutex , NULL) ;
	//pthread_cond_init(&send_data , NULL) ;  // 初始化条件变量
	//pthread_mutex_init(&sd_mutex , NULL) ;
	cout << "channel init" << endl ; 
}


Channel::Channel(int sockfd){
	this->sockfd = sockfd ;
	this->state = 1;
	//pthread_cond_init(&recv_data , NULL) ;  // 初始化条件变量
	//pthread_mutex_init(&rd_mutex , NULL) ;
	//pthread_cond_init(&send_data , NULL) ;  // 初始化条件变量
	//pthread_mutex_init(&sd_mutex , NULL) ;
}

Channel::~Channel(){

}

void Channel::start(){

}

void Channel::stop(){

}

//void Channel::lock_sd_mutex(){
	//pthread_mutex_lock(&(this->sd_mutex)) ;
//}

//void Channel::unlock_sd_mutex(){
	//pthread_mutex_unlock(&(this->sd_mutex)) ;
//}
