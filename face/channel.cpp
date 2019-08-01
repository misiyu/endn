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

