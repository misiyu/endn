#include <iostream>
#include "face.h"
#include "deamon/flist.h"

using namespace std;

Face::Face(const char *dip , int sockfd){
	daddr = dip ;
	mchannel = new Tcp_Channel(sockfd) ;
	m_fib = FIB::GetInstance() ;
	m_pit = PIT::GetInstance() ;
	m_flist = FList::GetInstance() ;
	this->m_state = ACTIVE ;

}

Face::Face(string &if_name, const uint8_t* s_mac ){
	daddr = if_name ;
	mchannel = new Ether_Channel(if_name,s_mac) ;
	m_fib = FIB::GetInstance() ;
	m_pit = PIT::GetInstance() ;
	m_flist = FList::GetInstance() ;
	this->m_state = ACTIVE ;
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

int Face::send2face(vector<int> &face_list , int clen){
	int face_list_n = face_list.size();
	for (int i = 0; i < face_list_n; i++) {
		m_flist->flist[face_list[i]]->add2chsq(
				this->mchannel->mrqueue.get_head_p(), clen);
	}
	(this->mchannel->mrqueue).rmv_n(clen) ;
}

int Face::add2chsq(char *data , int len){
	bool need_signal = this->mchannel->msqueue.is_empty() ;
	this->mchannel->msqueue.push_ndata(data,len);
	if(need_signal) pthread_cond_signal(&(this->mchannel->send_data)) ;
}

int Face::add2chrq(char *data , int len){
	bool need_signal = this->mchannel->mrqueue.is_empty() ;
	this->mchannel->mrqueue.push_ndata(data,len);
	if(need_signal) pthread_cond_signal(&(this->mchannel->recv_data)) ;
}

void *Face::search(void *param){
	Face *_this = (Face*)param ;
	
	while(1){
		pthread_mutex_lock(&(_this->mchannel->rd_mutex)) ;
		while(RQueue.is_empty()) {
			pthread_cond_wait(&(_this->mchannel->recv_data) ,
					&(_this->mchannel->rd_mutex)) ;
		}
		pthread_mutex_unlock(&(_this->mchannel->rd_mutex)) ;
		cout << "face get packet" << endl ;
		uint16_t name_len = 0 ;
		RQueue.get_ndata(RQueue.get_head()+4,(char*)&name_len,2);
		cout << "face get packet name_len = " << name_len << endl ;
		//exit(1) ;
		while( name_len+6 > RQueue.get_data_len() ) ; 
		cout << "get name data_len = " <<  RQueue.get_data_len() << endl ;
		char name[MAX_NAME_LEN];
		RQueue.get_ndata(RQueue.get_head()+3 , name , name_len+3);
		// 获得数据包名称: 两层 TLV 格式
		for (int i = 0; i < name_len+3; i++) {
			printf("%x " , name[i]) ;
		}
		printf("\n") ;
		//exit(0) ;
		vector<int> face_list = _this->m_pit->search(RQueue.get_head_p()+3) ;
		face_list = _this->m_fib->search(RQueue.get_head_p()+3) ;
		uint16_t packet_len = 0 ;
		RQueue.get_ndata(RQueue.get_head()+1,(char*)&packet_len,2) ;
		packet_len += 3 ;
		cout << "match face list size = " << face_list.size() << endl ;
		if(face_list.size() > 0){
			uint16_t need2send = packet_len ;
			while(need2send > 0){
				int clen = RQueue.get_cdata_len() ;
				if(clen > need2send) clen = need2send;
				need2send -= clen ;
				_this->send2face(face_list , clen) ;
			}
		}else{
			RQueue.rmv_n(packet_len) ;
		}
		//exit(0) ;
	}

}
