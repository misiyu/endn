#include <iostream>

#include "eface.h"

using namespace std;

int get_sockfd(){
	int sockfd ;
	int len ;
	struct sockaddr_un address ;
	int result ;
	int i,byte ;
	char ch_recv , ch_send ;
	if((sockfd = socket(AF_UNIX , SOCK_STREAM , 0)) == -1){
		printf("error\n");
		exit(1) ;
	}
	address.sun_family = AF_UNIX ;
	strcpy(address.sun_path,USOCKET_ID) ;
	len = sizeof(address) ;
	result = connect(sockfd , (struct sockaddr*)&address , len) ;
	if(result == -1){
		printf("ensure the server: %s is up\n", USOCKET_ID);
		exit(1) ;
	}
	return sockfd ;
}

EFace::EFace(){
	pthread_mutex_init(&send_q_mutex , NULL);
	pthread_cond_init(&send_data , NULL) ;  // 初始化条件变量
	state = 1 ;
	this->sockfd = get_sockfd();
	filter_str = new struct packet_t ;
	filter_str->data = (char*)malloc(MAX_P_SZ*5);
	//printf("%p\n" , filter_str) ;
	filter_str->len = 2 ;  // 前两个字节用来放长度
}

EFace::~EFace(){
	pthread_mutex_destroy(&send_q_mutex) ;
	close(sockfd) ;
}

// 发送一个兴趣包 : 将兴趣包加入发送队列 
void EFace::expressInterest(EInterest &einterest ) {
	struct packet_t *pkt = new struct packet_t ;
	pkt->len = einterest.get_packet(pkt->data);
	// get_packet 里面会给pkt->data分配内存
	//printf("expressInterest pkt->data = %p\n",pkt->data) ;  
	bool need_signal = false ;
	pthread_mutex_lock(&send_q_mutex);
	need_signal = send_q.empty() ;
	send_q.push(pkt);
	pthread_mutex_unlock(&send_q_mutex);
	//cout <<"pin 2 >>>>>>>>>>> expressInterest "  << einterest.getName() << endl ;
	if(need_signal) pthread_cond_signal(&(this->send_data)) ;
}

void EFace::set_saddr(string prefix , void(*onData)(const EData&) ){
	this->face_type = 'c' ;   // 监听源地址，为客户端
	interestFilter.insert(pair<string , void*>(prefix , (void*)onData)) ;
}

int EFace::m_recv(uint8_t *buff , int buff_len , int start){
	int rlen = start ;
	rlen += read(sockfd, buff+start, buff_len-start);   
	cout << "EFace::m_recv recv data len = " << rlen << endl ;
	if(rlen <= 0) exit(1) ;
	uint16_t p_len = *((uint16_t*)(buff+1)) +3;
	while(rlen < p_len){
		rlen += read(sockfd , buff+rlen , buff_len - rlen) ;
	}
	return rlen ;
}

void EFace::c_recv(){
	uint8_t buff[20000] ;
	int start = 0 ;
	int i = 0;
	void(*onData)(const EData&) ;
	uint16_t pkt_len = 0 ;
	while(state){   // state == 1 时表示活跃
		start = m_recv(buff , 20000 , start) ;
		pkt_len = *((uint16_t*)(buff+1)) + 3 ; 
		i = 0 ;
		while(i + pkt_len <= start){
			string name = decode_name(buff+i+3) ;
			uint16_t name_len = *((uint16_t*)(buff+i+4)) ;
			EData edata(name) ;
			uint16_t c_len = *((uint16_t*)(buff+i+11+name_len)) ;
			edata.setContent(buff+i+13+name_len , c_len) ;
			map<string,void*>::iterator it = interestFilter.find(name);
			if(it != interestFilter.end()){
				onData = (void(*)(const EData&))(it->second) ;
				onData(edata) ;
			}
			i += pkt_len ;
			if(start -i >= 3) {
				pkt_len = *((uint16_t*)(buff+i+1))+3 ; 
			}else{
				break ;
			}
		}
		start = start - i ;
		if(start > 0) memmove(buff, buff+i , start) ;
	}
}

void EFace::p_recv(){
	uint8_t buff[20000] ;
	int start = 0 ;
	int i = 0;
	uint16_t pkt_len = 0 ;
	void(*onInterest)(const EInterest &) ;
	while(state){   // state == 1 时表示活跃
		start = m_recv(buff , 20000, start) ;

		//for(int j = 0 ; j < start ; j++) printf("%x ",buff[j]) ;
		//cout << endl ;

		pkt_len = *((uint16_t*)(buff+1)) + 3 ; 
		i = 0 ;
		cout << "EFace::p_recv data len = " << start << endl ;
		while(i + pkt_len <= start){
			cout << "EFace::p_recv pkt len = " << pkt_len << endl ;
			string name1 = decode_name(buff+i+3) ;
			cout << "name1 = " << name1 << endl ;
			uint16_t name1_len = *((uint16_t*)(buff+i+4)) ;

			string name2 = decode_name(buff+i+6+name1_len) ;
			uint16_t name2_len = *((uint16_t*)(buff+i+7+name1_len)) ;

			EName ename1(name1.data()) ;
			EInterest einterest(ename1 , name2) ;
			uint16_t c_len = *((uint16_t*)(buff+i+14+name1_len+name2_len)) ;
			einterest.setContent(buff+i+16+name1_len + name2_len , c_len) ;
			map<string,void*>::iterator it = interestFilter.find(name1);
			if(it != interestFilter.end()){
				onInterest = (void(*)(const EInterest&))(it->second) ;
				onInterest(einterest) ;
			}
			i += pkt_len ;
			if(start -i >= 3) {
				pkt_len = *((uint16_t*)(buff+i+1)) + 3 ; 
			}else{
				break ;
			}
		}
		start = start - i ;
		if(start > 0) memmove(buff, buff+i , start) ;
	}
}

void send_cmd(struct packet_t *filter , int sockfd){
	int wlen = 0 ;
	while(wlen < filter->len){
		int tmp_len = write(sockfd, filter->data+wlen , filter->len - wlen) ;
		wlen += tmp_len ;
	}

}

void EFace::processEvents() {
	*((uint16_t*)filter_str->data) = filter_str->len -2 ;
	printf("processEvents %p\n",filter_str) ;
	send_cmd(filter_str , this->sockfd);
	int ret = pthread_create(&stid , NULL , send , (void*)this) ;
	if(this->face_type == 'c'){   // 消费者
		c_recv() ;
	}else{
		p_recv() ;
	} 
}

void EFace::put(EData &edata){
	struct packet_t *packet = new struct packet_t ;
	printf("put data name = s\n" ) ;
	packet->len = edata.get_packet(packet->data);
	bool need_signal = false ;
	pthread_mutex_lock(&send_q_mutex);
	need_signal = send_q.empty() ;
	send_q.push(packet);
	pthread_mutex_unlock(&send_q_mutex);
	if(need_signal) pthread_cond_signal(&(this->send_data)) ;
}

void EFace::shutdown(){
	state = 0;
	struct packet_t *stop_pkt = new struct packet_t ;
	stop_pkt->data = (char*)malloc(5) ;
	memcpy(stop_pkt->data , "stop" , 5) ;
	stop_pkt->len = 5 ;
	pthread_mutex_lock(&send_q_mutex);
	send_q.push(stop_pkt) ;
	pthread_mutex_unlock(&send_q_mutex);
}

void EFace::setInterestFilter(string prefix , 
		void (*onInterest)(const EInterest &)){
	this->face_type = 'p' ;
	interestFilter.insert(pair<string, void*>(prefix,(void*)onInterest));

	uint16_t prefix_len = enpacket(&(filter_str->data[filter_str->len]) , prefix ,
			0x7);
	filter_str->len += (prefix_len) ;
}

void *EFace::send(void *param){
	EFace *_this = (EFace*)param ;
	int max_bf_sz = MAX_P_SZ*10;
	char *send_buff = (char*)malloc(max_bf_sz) ;
	while(1){
		pthread_mutex_lock(&(_this->send_q_mutex)) ;
		while(_this->send_q.empty()) {
			pthread_cond_wait(&(_this->send_data) , &(_this->send_q_mutex)) ;
		}
		pthread_mutex_unlock(&(_this->send_q_mutex)) ;
		if(_this->state == 0) break ;
		int len = 0 ;

		pthread_mutex_lock(&_this->send_q_mutex);
		while((!_this->send_q.empty()) && len < max_bf_sz - MAX_P_SZ ){
			struct packet_t *pkt = _this->send_q.front() ;
			_this->send_q.pop();
			memcpy(send_buff+len , pkt->data , pkt->len);
			delete(pkt) ;
			len += pkt->len;
		}
		pthread_mutex_unlock(&_this->send_q_mutex);
		//for (int i = 0; i < len; i++) {
			//printf("%x ",send_buff[i]) ;
		//}
		int wlen = write(_this->sockfd, send_buff, len);   
		//数据发送到ENDND 包转发进程 
		cout << "send wlen = " << wlen << endl ;
	}
	free(send_buff);
}
