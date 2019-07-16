#include <iostream>
#include "flist.h"

using namespace std;

FList::FList(){
	for (int i = 0; i < FLIST_SZ; i++) {
		this->flist[i] = NULL ;
	}
	this->face_n = 0 ;
}

FList::~FList(){
	for (int i = 0; i <FLIST_SZ; i++) {
		if(this->flist[i] != NULL) {
			this->flist[i]->stop() ;
			delete(flist[i]);
		}
	}
}

int FList::get_new_fid(){
	int i = 0 ;
	for (i = 0; i < FLIST_SZ; i++) {
		if(flist[i] == NULL)
			break ;
		else if(flist[i]->get_state() == DEAD){
			flist[i]->stop() ;
			delete(flist[i]) ;
			flist[i] = NULL ;
			break ;
		}
	}
	if( i < FLIST_SZ ) return i ;
	else return -1 ;
}

int FList::s_add_tcp_face(char *cip , int sockfd){
	int faceid = get_new_fid();
	if(faceid < 0) return -1 ;
	flist[faceid] = new Face(cip , sockfd);
	face_n ++ ;
	return faceid ;
}
int FList::c_add_tcp_face(char *sip , int port){
	int faceid = get_new_fid();
	if(faceid < 0) return -1 ;
	int sockfd ;
	struct sockaddr_in server_addr ;
 
	bzero(&server_addr , sizeof(server_addr));
	server_addr.sin_family = AF_INET ;
	server_addr.sin_addr.s_addr = inet_addr(sip);
	server_addr.sin_port = htons(port);
	sockfd = socket(AF_INET , SOCK_STREAM , 0 );
	if(connect(sockfd , (struct sockaddr *)&server_addr , 
				sizeof(server_addr)) == -1){
		printf("connect to %s failed \n" , sip) ;
		return 0 ;
	}
	return s_add_tcp_face(sip , sockfd) ;
}
