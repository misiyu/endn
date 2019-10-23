#include <iostream>
#include "ether_recvs.h"

using namespace std;


// 功能：初始化网卡face列表
void Ether_RecvS::get_ifs(){
 
	FILE *fp = popen("ip link","r");
	char buff[300];
	uint8_t mac[6] ;
	int idx1 , idx2 ;
	string str_tmp ;
	while(fgets(buff,300,fp)!= NULL){
		// 获得网卡名及其index
		str_tmp = buff ;
		idx1 = str_tmp.find(':' , 0)+2 ; // 
		idx2 = str_tmp.find(':' , idx1) ;
		string if_name = str_tmp.substr(idx1,idx2-idx1) ;
		printf("if name = %s\n",if_name.data()) ;
		int if_idx = if_nametoindex(if_name.data()) ;
		printf("if id = %d\n", if_nametoindex(if_name.data())) ;

		memset(buff , 0 , sizeof(buff)) ;
		fgets(buff , sizeof(buff) , fp) ;
		str_tmp = buff ;
		int idx1 = str_tmp.find(':',0) ;
		idx1 -= 2 ;
		int itmp = 0 ;
		for (int i = 0; i < 6; i++) {
			sscanf(&buff[idx1+i*3] , "%x" , &itmp) ;
			mac[i] = itmp ;
			printf("%x:",mac[i]) ;
		}
		printf("%s",buff);
		memset(buff , 0 , sizeof(buff)) ;
		// 添加一个以太网类型的face
		m_flist->add_ether_face(if_name , mac , if_idx) ;
	}
	pclose(fp);
}

Ether_RecvS::Ether_RecvS(){
	this->m_flist = FList::GetInstance() ;
	get_ifs() ;
}

Ether_RecvS::~Ether_RecvS(){

}

void Ether_RecvS::start(){
	int ret = pthread_create(&(m_tid) , NULL , recv ,(void*)this) ;
}

void Ether_RecvS::stop(){

}

// 功能 ： 接收以太网数据包，并转发到相应的网卡对应的face中
void *Ether_RecvS::recv(void *param){
	cout << "Ether_RecvS thread recv start" << endl ;
	
	Ether_RecvS *_this = (Ether_RecvS*)param ;
	int fd = socket(PF_PACKET , SOCK_RAW , htons(NET_TYPE)) ;
	if(fd == -1){
		printf("start ether recv thread err\n");
		printf("maybe you are not root \n");
		exit(1);
	} 
	char buf[IP_MAXPACKET];
	struct sockaddr_ll device ;
	socklen_t sll_len = sizeof(struct sockaddr_ll) ;
	int recv_c = 0 ;
	while(1)
	{
		int bytes = recvfrom(fd, buf, sizeof(buf), 0 , (struct sockaddr*)&device, 
				&sll_len);
		printf("%d ether recv %d byte \n" , recv_c++ , bytes);
		//continue ;
		uint16_t d_len = bytes-14 ;
		int if_id = device.sll_ifindex ;
		int faceid = if_id ;
		//cout << "faceid = " << faceid << endl ;
		if(bytes <= 66){
			d_len = *((uint16_t*)(buf+14)) ;
			_this->m_flist->flist[faceid]->add2chrq(buf+16 , d_len);
		}else{
			_this->m_flist->flist[faceid]->add2chrq(buf+14 , d_len);
			//for (int i = 0; i < 15; i++) {
				//printf("%x ",buf[14+i]) ;
			//}
			//printf("\n") ;
		}
	}
	close(fd);
}
