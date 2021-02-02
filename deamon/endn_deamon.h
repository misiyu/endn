// create by wgh
#ifndef _ENDN_DEAMON_H_
#define _ENDN_DEAMON_H_

#include <sstream>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <jsoncpp/json/json.h>

//#include "common/globle_var.h"
#include "flist.h"
#include "deamon/tcp_server.h"
#include "deamon/usocket_server.h"
#include "ether_recvs.h"

#define PIPE_OUT "/tmp/endn_cmd_out"
#define PIPE_IN "/tmp/endn_cmd_pipe"

class Endn_Deamon
{
public:
	Endn_Deamon();
	~Endn_Deamon();
	void start() ;
	void recv_cmd() ;
	void update();
private:
	void route_cmd(std::stringstream &cmd_result, Json::Value &root) ;
	void face_cmd(std::stringstream &cmd_result,  Json::Value &root) ;
	void pit_cmd(std::stringstream &cmd_result,   Json::Value &root) ;

private:
	int exec_cmd(string cmd ) ;
	uint32_t mclock ;
	Tcp_Server mtcp_server ;
	USocket_Server musk_server ;
	Ether_RecvS ether_recvs ;
	FIB *m_fib ;
	PIT *m_pit ;
	FList *m_flist ;
	/* data */

};

#endif 
