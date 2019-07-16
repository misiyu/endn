#ifndef _ENDN_DEAMON_H_
#define _ENDN_DEAMON_H_

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "common/globle_var.h"
#include "flist.h"

#define P_FIFO "/tmp/endn_cmd_pipe"

class Endn_Deamon
{
public:
	Endn_Deamon();
	~Endn_Deamon();
	void start() ;
	void recv_cmd() ;
private:
	int exec_cmd(string cmd) ;
	/* data */

};

#endif 
