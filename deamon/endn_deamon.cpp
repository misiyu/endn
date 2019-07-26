#include <iostream>
#include "endn_deamon.h"
using namespace std;

Endn_Deamon::Endn_Deamon(){
	m_fib = FIB::GetInstance() ;
	m_pit = PIT::GetInstance() ;
	m_flist = FList::GetInstance() ;
	this->mclock = 0 ;
}

void Endn_Deamon::start(){
	//gflist.start() ;
	mtcp_server.start();
	musk_server.start() ;
	ether_recvs.start() ;
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

int Endn_Deamon::exec_cmd(string cmd ){
	Json::Reader reader ;
	Json::Value root ;
	reader.parse(cmd , root);

	string cmd1 = root["cmd1"].asString() ;
	string cmd2 = root["cmd2"].asString() ;

	stringstream cmd_result ;

	if(cmd1 == "route"){
		if(cmd2 == "add"){
			string prefix = root["prefix"].asString() ;
			int faceid = root["face_id"].asInt() ;
			if(m_flist->flist[faceid] == NULL || 
					m_flist->flist[faceid]->m_state != ACTIVE){
				cmd_result << "face not exist" ;
			}else{
				char en_name[MAX_NAME_LEN] ;
				encode_name(en_name, prefix) ;
				m_fib->add(en_name,faceid) ;
				cmd_result << "add route prefix " << prefix 
					<< " nexthop " << faceid ;
			}
		}else if(cmd2 == "remove"){

		}else{
			// get route list
			//map<string , struct fib_m>	
			string route_list = m_fib->get_table() ;
			if(route_list.size() != 0)
				cmd_result << route_list ;
			else
				cmd_result << "no record\n" ;
		}
	}else if(cmd1 == "face"){
		if(cmd2 == "add"){
			string remote_url = root["remote_url"].asString() ;
			int ind1 = remote_url.find(':',0)+3 ;
			if(remote_url[0] == 't'){
				int ind2 = remote_url.find(':',ind1) ;
				string sip = "" ;
				int port = 16363 ;
				if(ind2 == string::npos){
					sip = remote_url.substr(ind1 , remote_url.length()-ind1) ;
				}else{
					sip = remote_url.substr(ind1 , ind2-ind1) ;
					string port_str = remote_url.substr(ind2 +1 ,
							remote_url.length() - ind2 -1) ;
					port = atoi(port_str.data()) ;
				}
				int new_face = m_flist->c_add_tcp_face(sip.data() , port) ;
				cmd_result << "face-create id=" << new_face << 
					"remote="<<remote_url ;
				
			}else if(remote_url[0] == 'e'){

			}
		}else if(cmd2 == "remove"){

		}else{
			// get face list
			cout << "show face cmd " << endl ;
			string result = m_flist->get_flist_info() ;
			cmd_result << result ;
			cout << result << endl ;
		}
	}
	int fd = 0 ;
	fd = open(PIPE_OUT, O_WRONLY | O_NONBLOCK) ;
	// 发送结果
	if(cmd_result.str().size() == 0){
		string send_str = "no record" ;
		write(fd, send_str.data() , send_str.size()) ;
	}else{
		write(fd,cmd_result.str().data() , cmd_result.str().length()) ;
	}
	close(fd) ;
}

// 接收控制台发来的命令 + 定时更新一些对象
void Endn_Deamon::recv_cmd(){
	char cache[400];
	int fd ;
	memset(cache,0,sizeof(cache));
	if(access(PIPE_IN , F_OK) == 0){
		remove(PIPE_IN) ;
	}
	if(mkfifo(PIPE_IN, 0777) < 0){
		printf("create named pipe failed\n");
	}

	fd = open(PIPE_IN , O_RDONLY | O_NONBLOCK) ;
	while(1){
		memset(cache , 0 , sizeof(cache)) ;
		int rlen = read(fd,cache,400) ;
		//printf("get cmd %s \n",cache) ;
		if(rlen > 0){
			cout << "get cmd " << cache << endl ;
			exec_cmd(cache) ;
		}
		sleep(1);
		this->update() ; // 一些对象的状态
	}
	close(fd) ;
}
