#include <iostream>
#include "face.h"
#include "deamon/flist.h"

using namespace std;

// 功能： 构建TCP或unix_socket类型的face
// 参数：
//		  dip 目的IP
//		  sockfd  socket id get through connect or accept
//		  face_id  face index in the flist
Face::Face(const char *dip , int sockfd, int face_id):
	face_id(face_id) ,
	m_state(ACTIVE)
{
	daddr = dip ;
	if(daddr == "unix_socket"){
		m_ch_type = USC ;
	}else{
		m_ch_type = TCP ;
	}
	mchannel = new Tcp_Channel(sockfd) ;
	m_fib = FIB::GetInstance() ;
	m_pit = PIT::GetInstance() ;
	m_flist = FList::GetInstance() ;
}
enum CH_TYPE Face::get_ch_type() {
	return this->m_ch_type ;
}

// 功能： 构建ethernet类型的face
// 参数：
//		  if_name 网卡名称
//		  s_mac  网卡的mac地址
//		  face_id  face index in the flist
Face::Face(string &if_name, const uint8_t* s_mac , int face_id):
	face_id(face_id),
	m_state(ACTIVE),
	m_ch_type(ETH)
{
	char dmac[20] ;
	//sprintf(dmac , "%.2x" , s_mac)
	daddr = if_name + " -> 01:00:5e:00:17:aa" ;
	mchannel = new Ether_Channel(if_name,s_mac) ;
	m_fib = FIB::GetInstance() ;
	m_pit = PIT::GetInstance() ;
	m_flist = FList::GetInstance() ;
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
//			packet_len 收到的数据包长度
//			fhint 处理后的转发信息
//			fhint_l 处理后的转发信息长度 
//			need2send 接收到的包，除去转发信息部分，剩下的长度
//			
int Face::send2face(int face_id , int packet_len , uint8_t* fhint,
		int fhint_l , int need2send){
	R_Queue &RQueue = this->mchannel->mrqueue ;
	if(face_id >= 0){
		int remain_len = packet_len - need2send ;
		while(need2send > 0){
			int clen = RQueue.get_cdata_len() ;
			//cout << "clen = " << clen << endl ;
			if(clen > need2send) clen = need2send;
			need2send -= clen ;
			m_flist->flist[face_id]->add2chsq(
					RQueue.get_head_p(), clen);
			RQueue.rmv_n(clen) ;
		}
		m_flist->flist[face_id]->add2chsq(
				(char*)fhint, fhint_l);
		RQueue.rmv_n(remain_len) ;
	}else{
		RQueue.rmv_n(packet_len) ;
	}
	return 0 ;
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

static inline bool chk_type(const char& type){
	if(type != 5 && type != 6){
		printf("packet type error\n") ;
		exit(1) ;
	}
	return true ;
}


inline void Face::process_einterest(uint16_t daddr_len , const char *name,
		uint16_t packet_len){

	R_Queue& RQueue = (this->mchannel->mrqueue) ;
	int rq_head = RQueue.get_head() ;
	uint16_t saddr_len ;
	char saddr[MAX_NAME_LEN] ;
	vector<int> face_list ;
	while(RQueue.get_data_len() < 9+daddr_len ) ; 
	RQueue.get_ndata(rq_head + 7+daddr_len,(char*)&saddr_len,2) ;
	while(RQueue.get_data_len() < 9+daddr_len+saddr_len ) ; 
	RQueue.get_ndata(rq_head + 6+daddr_len,saddr, saddr_len+3) ;


	face_list = this->m_fib->search(name) ;

	//cout << "face_list size = " << face_list.size() << endl ;

	while(packet_len > RQueue.get_data_len()) ;  
	// 等待完整的包到达才转发出去
	if(face_list.size() <= 0) {
		send2face(-1, packet_len , NULL, 0, 0) ;
	}
	
	uint16_t new_pkt_len = packet_len -2  ;
	packet_len++ ;
	cout << packet_len << endl ;
	RQueue.insert_ndata(rq_head + 1 , (char*)&new_pkt_len,2) ;

		//for (int i = 0; i < 20; i++) {
			//printf("%d " , *(RQueue.get_head_p() + i) ) ;
		//}
		//printf("\n") ;

	uint16_t payload_l = 0 ; 
	RQueue.get_ndata(rq_head + 10 + daddr_len + saddr_len,(char*)&payload_l, 2) ;

	uint16_t fhint_l = 0 ; 
	RQueue.get_ndata(rq_head + 13 + daddr_len + saddr_len + payload_l ,
			(char*)&fhint_l, 2) ;
	fhint_l += 3 ;

	uint8_t fhint[300] ;
	RQueue.get_ndata(rq_head + 12 + daddr_len + saddr_len + payload_l ,
			(char*)fhint, fhint_l) ;

	//for (int i = 0; i < fhint_l; i++) {
		//printf("%d ",fhint[i]) ;
	//}
	//printf("\n") ;

	uint8_t ttl = fhint[3] ;
	(*((uint16_t*)(&fhint[1]))) ++ ;
	fhint_l ++ ;
	fhint[4+(0xff-ttl)] = this->face_id ;
	fhint[3] -- ;  // ttl -- 
	ttl -- ;

	//for (int i = 0; i < fhint_l; i++) {
		//printf("%d ",fhint[i]) ;
	//}
	//printf("\n") ;

	if(m_flist->flist[face_list[0]]->get_ch_type() == USC ) {
		fhint[3] = 0xff - fhint[3]+1 ;
		this->m_pit->add(saddr , (char*)fhint) ;
		fhint[3] = 0xff - fhint[3]+1 ;
	}

	uint16_t need2send = packet_len - fhint_l ;
	if( ttl > 0) {
		cout << "face_list[0] = " << face_list[0] << endl ;
		send2face(face_list[0], packet_len-1 , fhint, fhint_l , need2send) ;
	}else{
		send2face(-1, packet_len-1 , fhint, fhint_l, need2send) ;
	} 
}

inline void Face::process_edata(uint16_t daddr_len, const char* name, 
		uint16_t packet_len){

	R_Queue& RQueue = (this->mchannel->mrqueue) ;
	int rq_head = RQueue.get_head() ;
	while(packet_len > RQueue.get_data_len()) ;  
	// 等待完整的包到达才转发出去
	//
		//for (int i = 0; i < packet_len; i++) {
			//printf("%d " , *(RQueue.get_head_p() + i) ) ;
		//}
		//printf("\n") ;
	uint16_t payload_l = 0 ; ;
	RQueue.get_ndata(rq_head + 7 + daddr_len , (char*)&payload_l, 2) ;

	uint16_t fhint_l = 0 ; 
	RQueue.get_ndata(rq_head + 10 + daddr_len + payload_l ,
			(char*)&fhint_l, 2) ;
	fhint_l += 3 ;
	uint16_t need2send = packet_len - fhint_l ;
	uint16_t new_pkt_len  ;
	uint8_t fhint[300] ;
	RQueue.get_ndata(rq_head + 9 + daddr_len + payload_l ,
			(char*)fhint, fhint_l) ;
	uint8_t ttl = fhint[3] ;

	if(m_ch_type == USC ){
		string fhint_str = m_pit->search(name) ;
		
		//cout << endl ;
		//for (int i = 0; i < fhint_str.size(); i++) {
			//printf("%d ",fhint_str[i]) ;
		//}
		//cout << endl << "USC" << endl ;

		memcpy(fhint, fhint_str.data() , fhint_str.size()) ;
		fhint_l = *((uint16_t*)(&fhint[1])) + 3;
		ttl = fhint[3] ;
	}
	new_pkt_len = need2send + fhint_l-4 ;
	//packet_len-- ;
	RQueue.insert_ndata(rq_head + 1 , (char*)&new_pkt_len,2) ;



	(*((uint16_t*)(&fhint[1]))) -- ;
	fhint_l -- ;
	int next_face_id = fhint[2+ttl] ;
	fhint[3] -- ;  // ttl -- 
	ttl -- ;

	if(ttl > 0)
		send2face( next_face_id , packet_len , fhint, fhint_l,need2send) ;
	else 
		send2face(-1, packet_len , fhint, fhint_l , need2send) ;
}


// 功能 ： 线程函数，从face的接收数据队列中取出一个数据包，并进行pit和fib查找
//		   ，如果找到转发的下一个face，则将这个数据包加入到下一个face的send队列中
//		   
void *Face::forward(void *param){
	Face *_this = (Face*)param ;
	uint16_t daddr_len ;
	static int get_p_count = 0 ;
	char name[MAX_NAME_LEN];
	R_Queue& RQueue = (_this->mchannel->mrqueue) ;

	while(1){
		RQueue.wait4data() ;  
		/*
		 *wait for data , if receive queue is empty then wait 
		 *under the condition variant 
		 */
		cout << "data len = " << RQueue.get_data_len() << endl ;

		if(_this->mchannel->get_state() == 0){
			cout << "close face " << _this->daddr << endl ;
			_this->m_state = DEAD ;
			break ;
		}

		int rq_head = RQueue.get_head() ;

		uint8_t p_type ;
		RQueue.get_ndata(rq_head , (char*)&p_type,1) ;

		//printf("p_type = %hhx \n", p_type) ;
		//printf("face_id = %d \n", _this->face_id) ;

		//for (int i = 0; i < 20; i++) {
			//printf("%d " , *(RQueue.get_head_p() + i) ) ;
		//}
		//printf("\n") ;

		chk_type(p_type) ;

		// get addr's len
		while(  RQueue.get_data_len() < 6 ) ; 
		RQueue.get_ndata(rq_head + 4,(char*)&daddr_len,2);
		cout << "daddr_len = " << daddr_len << endl ;

		cout << "faceid = " << _this->face_id<<" face packet daddr_len = " << daddr_len << endl ;

		if(daddr_len == 0 || daddr_len > 500){
			printf("name len too long \n") ;
			exit(1) ;
		}

		// get addr value , name include its Type(1)|length(2)
		while(  RQueue.get_data_len() < daddr_len+6 ) ; 
		RQueue.get_ndata(rq_head + 3 , name , daddr_len+3);
		// 获得数据包名称: 两层 TLV 格式
		
		// get packet's len 
		uint16_t packet_len = 0 ;
		RQueue.get_ndata(rq_head + 1,(char*)&packet_len,2) ;
		packet_len += 3 ;
		
		//cout << "packet income : " << endl ;
		//for (int i = 0; i < 16; i++) {
			//printf("%c " , name[i]) ;
		//}
		//printf("\n") ;

		
		if(p_type == 0x5) {
			_this->process_einterest(daddr_len , name , packet_len) ;
		}else{
			_this->process_edata(daddr_len , name , packet_len) ;
		}

	}

}
