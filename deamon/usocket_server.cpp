#include <iostream>
#include "usocket_server.h"

using namespace std;

USocket_Server::USocket_Server(){
	m_fib = FIB::GetInstance();
	m_flist = FList::GetInstance() ;
}

USocket_Server::~USocket_Server(){

}

void USocket_Server::start(){
	printf("start usocket server ... \n") ;
	int ret = pthread_create(&(mtid) , NULL , run ,(void*)this) ;
}
void USocket_Server::stop(){

}

static void get_filter(vector<string> &filter , int sockfd){
	char buff[15000] ;
	int rlen = read(sockfd , buff , 15000);
	cout << "usocket rlen = " << rlen <<  endl ;
	uint16_t total_len = *((uint16_t*)buff) +2 ;
	cout << "usocket total_len = " << total_len <<  endl ;
	while(rlen < total_len){
		int len_tmp = read(sockfd , buff+rlen , 15000-rlen) ;
		rlen += len_tmp ;
	}
	for (int i = 0; i < rlen; i++) {
		printf("%x " , buff[i]) ;
	}
	printf("\n") ;
	int i = 3; 
	while(i < rlen){
		uint16_t p_len = *((uint16_t*)(buff+i)) ;
		string prefix(buff+i-1 , p_len +3) ;
		//cout << "get_filter prefix = " << prefix << endl ;
		//cout << "size of prefix = " << prefix.size() << endl ;
		//for(int j = 0 ; j < prefix.size() ; j++){
			//printf("%x " , prefix[j]) ;
		//}
		filter.push_back(prefix) ;
		i += (3+p_len) ;
	}
}

void *USocket_Server::run(void *param){

	USocket_Server *_this = (USocket_Server*)param ;

	int server_sockfd , client_sockfd ;
	int server_len , client_len ;
	struct sockaddr_un server_address ;
	struct sockaddr_un client_address ;
	int byte ;
	char ch_send , ch_recv ;
	unlink(USOCKET_ID);
	server_sockfd = socket(AF_UNIX , SOCK_STREAM , 0);
	server_address.sun_family = AF_UNIX ;
	strcpy(server_address.sun_path , USOCKET_ID) ;
	server_len = sizeof(server_address) ;

	bind(server_sockfd , (struct sockaddr*)&server_address,server_len);
	listen(server_sockfd, FLIST_SZ) ;
	printf("server: %s wait for client connect\n" , USOCKET_ID) ;
	client_len = sizeof(client_address);
	while(1){
		client_sockfd = accept(server_sockfd,(struct sockaddr*)&client_address,
				(socklen_t*)&client_len) ;
		printf("client connected\n") ;
		vector<string> filter;
		get_filter(filter , client_sockfd);
		int face_id = -1 ;
		while((face_id = _this->m_flist->s_add_tcp_face("localhost" ,
						client_sockfd)) == -1 ){
			sleep(1) ;
		}
		cout << "USocket_Server run face_id = " << face_id << endl ;
		int filter_sz = filter.size() ;
		cout << "filter size == " << filter_sz << endl ;
		for (int j = 0; j < filter_sz; j++) {
			//cout << "========================" << endl ;
			cout << "add a route entry : " << filter[j] << " face id = " << face_id  << endl ;
			_this->m_fib->add(filter[j].data() , face_id);
			// 添加到达这个face的路由
		}
		cout << " continue " << endl ;
	}
	unlink(USOCKET_ID);
}
