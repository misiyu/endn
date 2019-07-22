#ifndef _USOCKET_SERVER_H_
#define _USOCKET_SERVER_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

#include "flist.h"

#define USOCKET_ID "/tmp/ENDN_USOCKET"

class USocket_Server
{
public:
	USocket_Server();
	~USocket_Server();
	void start();
	void stop() ;
	static void *run(void *param);

private:
	/* data */
	pthread_t mtid ;
	FIB *m_fib ;
	FList *m_flist ;
};

#endif 
