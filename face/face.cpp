#include <iostream>
#include "face.h"

using namespace std;

Face::Face(char *dip , int sockfd){
	daddr = dip ;
	this->sockfd = sockfd ;
	mchannel = new Tcp_Channel(sockfd) ;
}

Face::~Face(){
	if(mchannel != NULL){
		delete(mchannel) ;
	}
}

void Face::start(){
	mchannel->start() ;
	int ret = pthread_create(&(mtid) , NULL , search ,(void*)this) ;
}

void Face::stop(){

}

int Face::send2face(vector<int> &face_list , int &clen){
	int face_list_n = face_list.size();
	for (int i = 0; i < face_list_n; i++) {
		gflist->flist[face_list[i]]->add2ch(this->mchannel->mrqueue.get_head_p(),
				clen);
	}
}

int Face::add2ch(char *data , int &len){
	this->mchannel->msqueue.push_ndata(data,len);
}

void *Face::search(void *param){
	Face *_this = (Face*)param ;
	
	while(1){
		while(RQueue.is_empty()) ;
		uint16_t name_len = 0 ;
		RQueue.get_ndata(RQueue.get_head()+4,(char*)&name_len,2);
		while( name_len < RQueue.get_data_len() ) ; 
		char name[MAX_NAME_LEN];
		RQueue.get_ndata(RQueue.get_head()+3 , name , name_len+3);
		// 获得数据包名称: 两层 TLV 格式
		vector<int> face_list = gpit.search(RQueue.get_head_p()) ;
		uint16_t packet_len = 0 ;
		RQueue.get_ndata(RQueue.get_head()+1,(char*)&packet_len,2);
		while(packet_len > 0){
			int clen = RQueue.get_cdata_len() ;
			if(clen > packet_len) clen = packet_len ;
			packet_len -= clen ;
			_this->send2face(face_list , clen) ;
			
		}
	}

}
