#include <iostream>
#include "face.h"
#include "deamon/flist.h"

using namespace std;

// 功能： 构建TCP或unix_socket类型的face
// 参数：
//		  dip 目的IP
//		  sockfd  socket id get through connect or accept
//		  face_id  face index in the flist
Face::Face(const char *dip , int sockfd, int face_id){
	daddr = dip ;
	this->face_id = face_id ;
	mchannel = new Tcp_Channel(sockfd) ;
	m_fib = FIB::GetInstance() ;
	m_pit = PIT::GetInstance() ;
	m_flist = FList::GetInstance() ;
	this->m_state = ACTIVE ;

}

// 功能： 构建ethernet类型的face
// 参数：
//		  if_name 网卡名称
//		  s_mac  网卡的mac地址
//		  face_id  face index in the flist
Face::Face(string &if_name, const uint8_t* s_mac , int face_id){
	char dmac[20] ;
	//sprintf(dmac , "%.2x" , s_mac)
	daddr = if_name + " -> 01:00:5e:00:17:aa" ;
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

// 功能 ： 启动channel 传输线程以及包转发线程forward
void Face::start(){
	mchannel->start() ;
	int ret = pthread_create(&(mtid) , NULL , forward ,(void*)this) ;
}

void Face::stop(){
	this->m_state = DEAD ;
}

// 功能: 将接收队列中，连续长度为clen的数据发送到多个face中
// 参数: face_list 发送出去的face列表
//			clen 数据长度
//			
int Face::send2face(vector<int> &face_list , int clen){
	int face_list_n = face_list.size();
	for (int i = 0; i < face_list_n; i++) {
		m_flist->flist[face_list[i]]->add2chsq(
				this->mchannel->mrqueue.get_head_p(), clen);
	}
	(this->mchannel->mrqueue).rmv_n(clen) ;
}

// 功能： 将长度为len的数据添加到本face的发送队列中
// 参数： data  buffer指针
//			len 数据长度
int Face::add2chsq(char *data , int len){
	//cout << "face : send to send queue " << endl ;
	this->mchannel->msqueue.push_ndata(data,len);
}

// 功能： 将长度为len的数据添加到本face的接收队列中（以太网face用到这个函数）
// 参数： data  buffer指针
//			len 数据长度
int Face::add2chrq(char *data , int len){
	this->mchannel->mrqueue.push_ndata(data,len);
}

void Face::set_dmac(const uint8_t *dmac) {
	printf("face set_dmac dmac = %p\n" , dmac) ;
	this->mchannel->set_dmac(dmac) ;
	char dmac_str[20];
	for (int i = 0; i < 5; i++) {
		sprintf(dmac_str+i*3,"%.2x:",dmac[i]);
	}
	sprintf(dmac_str+15,"%.2x",dmac[5]) ;
	int idx1 = this->daddr.find('>',0) ;
	daddr = daddr.substr(0,idx1+1)+" "+dmac_str ;
}


// 功能 ： 线程函数，从face的接收数据队列中取出一个数据包，并进行pit和fib查找
//		   ，如果找到转发的下一个face，则将这个数据包加入到下一个face的send队列中
//		   
void *Face::forward(void *param){
	Face *_this = (Face*)param ;
	char saddr[MAX_NAME_LEN] ;
	uint16_t saddr_len ;
	static int get_p_count = 0 ;

	while(1){
		RQueue.wait4data() ;  
		/*
		 *wait for data , if receive queue is empty then wait 
		 *under the condition variant 
		 */
		if(_this->mchannel->get_state() == 0){
			cout << "close face " << _this->daddr << endl ;
			_this->m_state = DEAD ;
			break ;
		}
		//cout << "Face::forward packet ================ " << get_p_count <<  endl ;
		while(  RQueue.get_data_len() < 6 ) ; 
		uint16_t name_len = 0 ;
		int rq_head = RQueue.get_head() ;
		RQueue.get_ndata(rq_head + 4,(char*)&name_len,2);
		//cout << "buff data total len = " <<  RQueue.get_data_len() << endl ;
		while( name_len+6 > RQueue.get_data_len() ) ; 
		char name[MAX_NAME_LEN];
		cout << "faceid = " << _this->face_id<<" face packet name_len = " << name_len << endl ;
		if(name_len == 0){
			exit(1) ;
		}
		RQueue.get_ndata(rq_head + 3 , name , name_len+3);
		// 获得数据包名称: 两层 TLV 格式
		uint16_t packet_len = 0 ;
		RQueue.get_ndata(rq_head + 1,(char*)&packet_len,2) ;
		packet_len += 3 ;

		//cout << "Face::search pkt len = " << packet_len << endl ;

		vector<int> face_list ;
		
		uint8_t p_type ;
		RQueue.get_ndata(rq_head , (char*)&p_type,1) ;
		if(p_type == 0x5) {
			while(RQueue.get_data_len() < 9+name_len ) ; 
			RQueue.get_ndata(rq_head + 7+name_len,(char*)&saddr_len,2) ;
			while(RQueue.get_data_len() < 9+name_len+saddr_len ) ; 
			RQueue.get_ndata(rq_head + 6+name_len,saddr, saddr_len+3) ;
			_this->m_pit->add(saddr , _this->face_id) ;
			face_list = _this->m_fib->search(name) ;
			//cout<<"fib match face list size = "<<face_list.size() << endl ;
		}else{
			face_list = _this->m_pit->search(name) ;
			cout<<"pit match face list size = "<<face_list.size() << endl ;
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
		//cout << "face get packet ==================== " << get_p_count++ <<  endl ;
	}

}
