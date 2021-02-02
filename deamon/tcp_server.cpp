// create by wgh
#include <iostream>
#include "tcp_server.h"

using namespace std;

Tcp_Server::Tcp_Server(){
	m_flist = FList::GetInstance() ;
}
Tcp_Server::~Tcp_Server(){
}
void Tcp_Server::start(){
	printf("start tcp server ... \n") ;
	int ret = pthread_create(&(mtid) , NULL , run ,(void*)this) ;
}
void Tcp_Server::stop(){

}

void* Tcp_Server::run(void *param){
	Tcp_Server *_this = (Tcp_Server*)param ;
	int ser_sockfd;
	struct sockaddr_in cli_addr , ser_addr;
	int port = TCP_PORT ;

	ser_sockfd = socket(AF_INET,SOCK_STREAM,0);
	bzero((char*)&ser_addr , sizeof(ser_addr));
	
	ser_addr.sin_family = AF_INET ;
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ser_addr.sin_port = htons(port);
	if(bind(ser_sockfd , (struct sockaddr*)&ser_addr , sizeof(ser_addr)) == -1){
		printf("Bind error:%d\n", port);
		exit(1);
	}

	printf("listening on port : %d\n",port) ;
	int listen_list_sz = FLIST_SZ ;
	listen(ser_sockfd , listen_list_sz) ;
	while(1){
		socklen_t cli_addr_len = sizeof(cli_addr) ;
		int new_sockfd = accept(ser_sockfd,(struct sockaddr*)&cli_addr ,
				&cli_addr_len);
		cout << "new tcp socket accept " << new_sockfd << endl ;
		char *cip = inet_ntoa(cli_addr.sin_addr) ;
		while(_this->m_flist->s_add_tcp_face(cip ,new_sockfd) == -1 ){
			sleep(1) ;
		}
		
	}
}
