#include <iostream>
#include "r_queue.h"

using namespace std;
R_Queue::R_Queue(){
	this->head = 0 ;
	this->rear = 0 ;
	pthread_cond_init(&has_data , NULL) ;
	pthread_mutex_init(&m_mutex , NULL) ;
}
R_Queue::~R_Queue(){

}
int R_Queue::get_head(){
	return head ;
}
int R_Queue::get_rear(){
	return rear ;
}
bool R_Queue::is_full(){
	return ((this->rear+1)%QUEUE_SZ == this->head) ;
}
bool R_Queue::is_empty(){
	bool ret = false ;
	pthread_mutex_lock(&m_mutex) ;
	ret = (this->head == this->rear) ;
	pthread_mutex_unlock(&m_mutex) ;
	return ret ;
}
int R_Queue::add_n(int n){
	bool need_signal = false ;
	pthread_mutex_lock(&m_mutex) ;
	need_signal = (head == rear) ;
	rear = (rear + n) % QUEUE_SZ ;
	pthread_mutex_unlock(&m_mutex) ;
	if(need_signal) pthread_cond_signal(&has_data) ;
}
int R_Queue::rmv_n(int n){
	pthread_mutex_lock(&m_mutex) ;
	head = (head + n) % QUEUE_SZ ;
	pthread_mutex_unlock(&m_mutex) ;
}
char *R_Queue::get_head_p(){
	return &(buff[this->head]);
}
char *R_Queue::get_rear_p(){
	return &(buff[this->rear]) ;
}
// 得到所有数据的长度
int R_Queue::get_data_len(){
	return (rear - head + QUEUE_SZ)%QUEUE_SZ ;
}
// 得到空余长度
int R_Queue::get_free_space(){
	return (head-rear-1+QUEUE_SZ)%QUEUE_SZ ;
}
// get the length of a continuous free space
// 获得从rear开始往后的连续剩余空间的长度 
int R_Queue::get_cfree_space(){
	int space = 0 ;
	if(head <= rear ) {
		space = QUEUE_SZ - rear ;
		if(head == 0) space -= 1 ;
	}else{
		space = head - rear - 1 ;
	}
	return space ;
}
// begin at head , get the continuous data block len in ring queue
int R_Queue::get_cdata_len(){
	int len = 0;
	if(rear < head){
		len = QUEUE_SZ - head ;
	}else{
		len = rear-head ;
	}
	return len ;
}

// copy n Byte data from buff to data 
// 从start开始，往后取n字节数据
int R_Queue::get_ndata(int start , char *data ,int n ){
	if(start + n >= QUEUE_SZ){
		memcpy(data,&buff[start],QUEUE_SZ-start);
		memcpy(&data[QUEUE_SZ-start],buff,n+start-QUEUE_SZ);
	}else{
		memcpy(data,&buff[start],n);
	}
}
// 队列中加入n字节数据
int R_Queue::push_ndata(char *data , int n){
	//if(get_free_space() < n) return -1 ;	
	while(get_free_space() < n ) ;
	int clen ;
	while(n > 0){
		clen = get_cfree_space() ;
		if(clen > n) clen = n;
		memcpy(&buff[rear],data,clen);
		add_n(clen);
		n -= clen ;
	}
}
