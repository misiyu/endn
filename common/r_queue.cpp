#include <iostream>
#include "r_queue.h"

using namespace std;
R_Queue::R_Queue(){
	this->head = 0 ;
	this->rear = 0 ;
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
	return (this->head == this->rear) ;
}
int R_Queue::add_n(int n){
	//for (int i = 0; i < n; i++) {
		//int j = (rear+i)%QUEUE_SZ ;
		//printf("%x ",buff[j]) ;
	//}
	rear = (rear + n) % QUEUE_SZ ;
}
int R_Queue::rmv_n(int n){
	head = (head + n) % QUEUE_SZ ;
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
int R_Queue::get_cfree_space(){
	int space = 0 ;
	if(head <= rear ) {
		space = QUEUE_SZ - rear ;
		if(head == 0) space - 1 ;
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
	if(get_free_space() < n) return -1 ;	
	int clen ;
	while(n > 0){
		clen = get_cfree_space() ;
		if(clen > n) clen = n;
		memcpy(&buff[rear],data,clen);
		add_n(clen);
		n -= clen ;
	}
}
