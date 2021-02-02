// create by wgh
#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include "flist.h"

#define TCP_PORT 16363

class Tcp_Server
{
public:
	Tcp_Server();
	~Tcp_Server();
	void start();
	void stop();
	static void *run(void *param) ;
	

private:
	/* data */
	pthread_t mtid ;
	FList *m_flist ;
};

#endif 
