#include <iostream>
#include "endn_deamon.h"
using namespace std;


Endn_Deamon::Endn_Deamon(){
	void_gflist = new FList();
}

void Endn_Deamon::start(){
	//gflist.start() ;
}

Endn_Deamon::~Endn_Deamon(){
}

void Endn_Deamon::recv_cmd(){
	char cache[400];
	int fd ;
	memset(cache,0,sizeof(cache));
	if(access(P_FIFO , F_OK) == 0){
		remove(P_FIFO) ;
	}
	if(mkfifo(P_FIFO, 0777) < 0){
		printf("create named pipe failed\n");
	}

	//fd = open(P_FIFO , O_RDONLY | O_NONBLOCK) ;
	while(1){
		memset(cache , 0 , sizeof(cache)) ;
		read(fd,cache,100) ;
		printf("get data %s \n",cache) ;
		sleep(1);
	}
	close(fd) ;
}
