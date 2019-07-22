#ifndef _R_QUEUE_H_
#define _R_QUEUE_H_

#include <string.h>
#define QUEUE_SZ 8000000

class R_Queue
{
public:
	R_Queue() ;
	virtual ~R_Queue();
	int get_head();
	int get_rear();
	bool is_full();
	bool is_empty();
	int add_n(int n);
	int rmv_n(int n);
	char *get_head_p();
	char *get_rear_p();

	// 得到所有数据的长度;
	int get_data_len();

	// 得到空余长度;
	int get_free_space();

	// get the length of a continuous free space;
	int get_cfree_space();

	// begin at head , get the continuous data block len in ring queue;
	int get_cdata_len();

	// copy n Byte data from buff to data ;
	// 从start开始，往后取n字节数据;
	int get_ndata(int start , char *data ,int n );


	// 队列中加入n字节数据;
	int push_ndata(char *data , int n);
protected:
	int rear ;
	int head ;
	char buff[QUEUE_SZ];

private:
	/* data */

};

#endif 
