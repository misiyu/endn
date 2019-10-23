#ifndef _EFACE_H_
#define _EFACE_H_

#include <queue>
#include <map>
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

#include "einterest.h"
#include "edata.h"

#define USOCKET_ID "/tmp/ENDN_USOCKET"

using std::queue ;
using std::map ;
using std::pair ;
using std::string ;

class EFace
{
public:
	EFace();
	~EFace();

	// 发送一个兴趣包
	void expressInterest(EInterest &einterest  ) ;
	void processEvents() ;
	void shutdown();
	void setInterestFilter( string prefix , void(*onInterest)(const EInterest &));
	void set_saddr( string prefix ,void(*onData)(const EData&) );
	void put(EData &edata) ;

	// 将发送队列里的数据发送出去
	static void *send(void *param) ;

	int m_recv(uint8_t *buff , int buff_len , int start) ;  
	void c_recv() ;  // 消费者的接收包程序
	void p_recv() ;  // 生产者的接收包程序
private:
	/* data */
	queue<struct packet_t*> send_q ;
	pthread_t stid ;
	pthread_mutex_t send_q_mutex ;   // 发送队列读写锁
	pthread_cond_t send_data ;  // 发送队列中有数据，条件变量

	map<string, void*> interestFilter ;

	struct packet_t * filter_str ;
	// 格式 [2:载荷长度][ [2:单个前缀长度] [前缀] ] 
	char face_type ;   // c = 消费者  p = 生产者
	int sockfd ;
	int state ;
};

#endif 
