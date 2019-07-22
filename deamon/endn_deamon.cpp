#include <iostream>
#include "endn_deamon.h"
using namespace std;

Endn_Deamon::Endn_Deamon(){
	m_fib = FIB::GetInstance() ;
	m_pit = PIT::GetInstance() ;
	this->mclock = 0 ;
}

void Endn_Deamon::start(){
	//gflist.start() ;
	mtcp_server.start();
	musk_server.start() ;
	recv_cmd() ;
}

// 更新一些对象的状态。FIB、PIT、FList 等 
void Endn_Deamon::update(){
	mclock ++ ;
	if(mclock % 5 == 0 ) m_fib->update();
	m_pit->update();
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

	fd = open(P_FIFO , O_RDONLY | O_NONBLOCK) ;
	while(1){
		memset(cache , 0 , sizeof(cache)) ;
		read(fd,cache,100) ;
		//printf("get cmd %s \n",cache) ;
		sleep(1);
		this->update() ; // 一些对象的状态
	}
	close(fd) ;
}
