// create by wgh
#include <iostream>
#include "channel.h"

using namespace std;

Channel::Channel(){
	this->state = 1;
	cout << "channel init" << endl ; 
}


Channel::Channel(int sockfd){
	this->sockfd = sockfd ;
	this->state = 1;
}

Channel::~Channel(){

}

void Channel::start(){

}

void Channel::stop(){

}

int Channel::get_state(){
	return this->state ;
}

void Channel::set_dmac(const uint8_t *d_mac){

}
