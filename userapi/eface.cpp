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
	state = 0 ;
	this->sockfd = get_sockfd();
	filter_str = new struct packet_t ;
	filter_str->data = (char*)malloc(MAX_P_SZ*5);
	printf("%p\n" , filter_str) ;
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
	printf("expressInterest pkt->data = %p\n",pkt->data) ;  
	bool need_signal = send_q.empty() ;
	pthread_mutex_lock(&send_q_mutex);
	send_q.push(pkt);
	pthread_mutex_unlock(&send_q_mutex);
	if(need_signal) pthread_cond_signal(&(this->send_data)) ;
}

void EFace::set_saddr(string prefix , void(*onData)(const EData&) ){
	this->face_type = 'c' ;   // 监听源地址，为客户端
	interestFilter.insert(pair<string , void*>(prefix , (void*)onData)) ;
}

void EFace::m_recv(){
	char buff[80000] ;
	int rlen = read(sockfd, buff, 80000);   
	cout << "recv data len = " << rlen << endl ;
	if(rlen <= 0) exit(1) ;
}

void EFace::c_recv(){
	while(!state){   // state == 0 时表示活跃
		m_recv() ;
		sleep(1) ;
		//cout << "face recv" << endl ;
	}
}

void EFace::p_recv(){
	while(!state){   // state == 0 时表示活跃
		m_recv() ;
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

void EFace::put(EData &data){
	struct packet_t *packet = new struct packet_t ;
	packet->len = data.get_packet(packet->data);
	bool need_signal = send_q.empty() ;
	pthread_mutex_lock(&send_q_mutex);
	send_q.push(packet);
	pthread_mutex_unlock(&send_q_mutex);
	if(need_signal) pthread_cond_signal(&(this->send_data)) ;
}

void EFace::shutdown(){
	state = 1;
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
	int max_bf_sz = MAX_P_SZ*100;
	char *send_buff = (char*)malloc(max_bf_sz) ;
	while(1){
		pthread_mutex_lock(&(_this->send_q_mutex)) ;
		while(_this->send_q.empty()) {
			pthread_cond_wait(&(_this->send_data) , &(_this->send_q_mutex)) ;
		}
		pthread_mutex_unlock(&(_this->send_q_mutex)) ;
		if(_this->state == 1) break ;
		int len = 0 ;

		cout << "face send" << endl ;

		pthread_mutex_lock(&_this->send_q_mutex);
		cout << "face send 2" << endl ;
		while((!_this->send_q.empty()) && len < max_bf_sz - MAX_P_SZ ){
			struct packet_t *pkt = _this->send_q.front() ;
			_this->send_q.pop();
			memcpy(send_buff+len , pkt->data , pkt->len);
			delete(pkt) ;
			len += pkt->len;
		}
		pthread_mutex_unlock(&_this->send_q_mutex);
		for (int i = 0; i < len; i++) {
			printf("%x ",send_buff[i]) ;
		}
		int wlen = write(_this->sockfd, send_buff, len);   
		//数据发送到ENDND 包转发进程 
		cout << "send wlen = " << wlen << endl ;
	}
	free(send_buff);
}
