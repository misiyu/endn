#include <iostream>
#include "endndc.h"

#define P_FIFO "/tmp/endn_cmd_pipe"

using namespace std;

int send_cmd(string cmd){
	int fd = 0 ;
	fd = open(P_FIFO , O_WRONLY | O_NONBLOCK) ;
	write(fd,cmd.data(),cmd.length()) ;
	close(fd) ;
}

int main()
{
	cout <<"Hello world!" << endl;

	return 0;
}
