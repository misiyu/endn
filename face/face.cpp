#include <iostream>
#include "face.h"
#include "deamon/flist.h"

using namespace std;

Face::Face(const char *dip , int sockfd, int face_id){
	daddr = dip ;
	this->face_id = face_id ;
	mchannel = new Tcp_Channel(sockfd) ;
	m_fib = FIB::GetInstance() ;
	m_pit = PIT::GetInstance() ;
	m_flist = FList::GetInstance() ;
	this->m_state = ACTIVE ;

}

Face::Face(string &if_name, const uint8_t* s_mac , int face_id){
	daddr = if_name ;
	this->face_id = face_id ;
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
	//pthread_mutex_lock(&(this->mchannel->rd_mutex)) ;
	(this->mchannel->mrqueue).rmv_n(clen) ;
	//pthread_mutex_unlock(&(this->mchannel->rd_mutex)) ;
}

int Face::add2chsq(char *data , int len){
	cout << "face : send to send queue " << endl ;
	//bool need_signal = this->mchannel->msqueue.is_empty() ;
	this->mchannel->msqueue.push_ndata(data,len);
	//if(need_signal) { 
		//pthread_cond_signal(&(this->mchannel->send_data)) ; 
		//cout << "wake up send thread " << endl ;
	//}
}

int Face::add2chrq(char *data , int len){
	//bool need_signal = this->mchannel->mrqueue.is_empty() ;
	this->mchannel->mrqueue.push_ndata(data,len);
	//if(need_signal) pthread_cond_signal(&(this->mchannel->recv_data)) ;
}


// 功能 ： 线程函数，从face的接收数据队列中取出一个数据包，并进行pit和fib查找
//		   ，如果找到转发的下一个face，则将这个数据包加入到下一个face的send队列中
//		   
void *Face::search(void *param){
	Face *_this = (Face*)param ;
	char saddr[MAX_NAME_LEN] ;
	uint16_t saddr_len ;

	static int get_p_count = 0 ;

	//while(1){
		//usleep(200) ;
	//}

	while(1){
		pthread_mutex_lock(&(_this->mchannel->rd_mutex)) ;
		while(RQueue.is_empty()) {
			pthread_cond_wait(&(_this->mchannel->recv_data) ,
					&(_this->mchannel->rd_mutex)) ;
		}
		pthread_mutex_unlock(&(_this->mchannel->rd_mutex)) ;
		cout << "Face::search packet ==================================== " << get_p_count <<  endl ;
		uint16_t name_len = 0 ;
		RQueue.get_ndata(RQueue.get_head()+4,(char*)&name_len,2);
		//cout << "face packet name_len = " << name_len << endl ;
		cout << "buff data total len = " <<  RQueue.get_data_len() << endl ;
		while( name_len+6 > RQueue.get_data_len() ) ; 
		char name[MAX_NAME_LEN];
		RQueue.get_ndata(RQueue.get_head()+3 , name , name_len+3);
		// 获得数据包名称: 两层 TLV 格式
		//for (int i = 0; i < name_len+3; i++) {
			//printf("%x " , name[i]) ;
		//}
		//printf("\n") ;
		//exit(0) ;
		uint16_t packet_len = 0 ;
		RQueue.get_ndata(RQueue.get_head()+1,(char*)&packet_len,2) ;
		packet_len += 3 ;

		cout << "Face::search pkt len = " << packet_len << endl ;

		vector<int> face_list ;
		
		uint8_t p_type ;
		RQueue.get_ndata(RQueue.get_head(),(char*)&p_type,1) ;
		if(p_type == 0x5) {
			RQueue.get_ndata(RQueue.get_head()+7+name_len,(char*)&saddr_len,2) ;
			RQueue.get_ndata(RQueue.get_head()+6+name_len,saddr, saddr_len+3) ;
			_this->m_pit->add(saddr , _this->face_id) ;
			face_list = _this->m_fib->search(name) ;
			//cout<<"fib match face list size = "<<face_list.size() << endl ;
		}else{
			face_list = _this->m_pit->search(name) ;
			//cout<<"pit match face list size = "<<face_list.size() << endl ;
		}

		//cout << "face search pkt len = " << packet_len << endl ;
		while(packet_len > RQueue.get_data_len()) ;  
		// 等待完整的包到达才转发出去

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
		cout << "face get packet ==================================== " << get_p_count++ <<  endl ;
		//exit(0) ;
	}

}
