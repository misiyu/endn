#include <iostream>
#include "flist.h"

using namespace std;

// 全局共用一个facelist 所以使用单例模式 
FList *FList::GetInstance(){
	if(m_flist == NULL){
		m_flist = new FList() ;
	}
	return m_flist ;
}

FList::FList(){
	cout << "flist init " << endl ;
	for (int i = 0; i < FLIST_SZ; i++) {
		this->flist[i] = NULL ;
	}
	printf("flist flist p =  %p\n", this->flist) ;
	this->face_n = 0 ;
}

FList::~FList(){
	cout << "flist destroy " << endl ;
	for (int i = 0; i <FLIST_SZ; i++) {
		if(this->flist[i] != NULL) {
			this->flist[i]->stop() ;
			delete(flist[i]);
		}
	}
}


// 功能 ： 在facelist中获得一个新的fid
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

// 功能： 在服务器侧被动添加一个face
// cip 为发起连接的主机的地址
// 返回值  new face id
int FList::s_add_tcp_face(const char *cip , int sockfd){
	cout <<" flist cip = " << cip <<  endl ;
	printf("flist get new fid %p\n", this->flist) ;
	int faceid = get_new_fid();
	cout << "flist new face id = " << faceid ;
	if(faceid < 0) return -1 ;
	flist[faceid] = new Face(cip , sockfd);
	flist[faceid]->start();
	face_n ++ ;
	return faceid ;
}

// 功能：在发起连接一侧，主动添加一个TCP的face
// sip 为服务器的IP地址
// 返回值  new face id
int FList::c_add_tcp_face(const char *sip , int port){
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

int FList::add_ether_face(string &if_name, uint8_t *s_mac ){
	int faceid = get_new_fid() ;
	if(faceid < 0 ) return -1 ;
	flist[faceid] = new Face(if_name , s_mac) ;
	flist[faceid]->start() ;
	face_n ++ ;
	return faceid ;
}

// 功能：查看所有face的信息
string FList::get_flist_info(){
	std::stringstream ss ;
	for (int i = 0; i < FLIST_SZ; i++) {
		if(flist[i] != NULL && flist[i]->m_state == ACTIVE ){
			ss << "faceid=" << i << " remote="<<flist[i]->daddr <<"\n" ;
		}
	}
	return ss.str() ;
}
