#ifndef _CHANNEL_H_
#define _CHANNEL_H_
#include "tcp_rqueue.h"  
#include "tcp_squeue.h"  

class Channel
{
public:
	Channel() ;
	Channel(int sockfd) ;

	void set_sockfd(int sockfd) ;
	virtual ~Channel() ;
	//int send(){ }
	//int recv(){ }
	virtual void start() ;
	virtual void stop() ;
	Tcp_RQueue mrqueue ;
	Tcp_SQueue msqueue ;
	pthread_cond_t recv_data ;   // 接收数据等待，条件变量
	pthread_mutex_t rd_mutex ;   // 以上条件变量的互斥锁

	pthread_cond_t send_data ;   // 接收数据等待，条件变量
	pthread_mutex_t sd_mutex ;   // 以上条件变量的互斥锁
protected:
	int state ;
	int sockfd ;
	pthread_t send_td ;
	pthread_t recv_td ;
private:
	/* data */
};

#endif 
