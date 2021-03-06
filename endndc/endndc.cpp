// create by wgh
#include <iostream>
#include <regex>
#include "endndc.h"

#define PIPE_OUT "/tmp/endn_cmd_out"
#define PIPE_IN "/tmp/endn_cmd_pipe"

using namespace std;

// 发送数据到转发进程
int send_cmd(const string cmd){
	//cout << "send cmd " << cmd << endl ;
	int fd = 0 ;
	fd = open(PIPE_IN, O_WRONLY | O_NONBLOCK) ;
	// 发送命令
	write(fd,cmd.data(),cmd.length()) ;
	close(fd) ;

	//cout << "send cmd finish" << endl ;

	// 接收结果
	char result[1000] ;
	memset(result,0,sizeof(result)) ;
	if(access(PIPE_OUT , F_OK) == 0){
		remove(PIPE_OUT) ;
		unlink(PIPE_OUT) ;
	}
	if(mkfifo(PIPE_OUT, 0777) < 0){
		printf("create named pipe failed\n");
	}
	fd = open(PIPE_OUT , O_RDONLY | O_NONBLOCK) ;
	while(1){
		memset(result , 0 , sizeof(result)) ;
		int rlen = read(fd,result,400) ;
		if(rlen > 0){
			cout << result << endl ;
			break ;
		}
		sleep(1);
	}
	close(fd) ;
}

void show_usage(){
	cout << "Usage : ./endndc route " << endl ;
	cout << "Usage : ./endndc face " << endl ;
	cout << "Usage : ./endndc pit " << endl ;
	cout << "./endndc -h for more detail help" << endl ;
	exit(1) ;
}
void show_route_usage(){
	cout << "./endndc route add <prefix> <face id>" << endl ;
	cout << "./endndc route remove <prefix> [face id]" << endl ;
	cout << "./endndc route list" << endl ;
	exit(1) ;
}
void show_face_usage(){
	cout << "./endndc face add tcp://<IP> " << endl ;
	cout << "./endndc face add ether://<MAC> dev://<if_name> " << endl ;
	cout << "./endndc face remove [face id]" << endl ;
	cout << "./endndc face list" << endl ;
	exit(1) ;
}

void route_cmd(int argc , char **argv){
	Json::Value root ;
	root["cmd1"]="route" ;
	if(argc == 2){
		root["cmd2"] = "list" ;
	}else{
		string cmd2 = argv[2] ;
		root["cmd2"] = cmd2;
		if(cmd2 == "add"){
			if(argc != 5) show_route_usage() ;
			string prefix = argv[3] ;
			root["prefix"] = prefix ;
			int face_id = atoi(argv[4]) ;
			root["face_id"] = face_id ;
		}else if(cmd2 == "remove"){
			if(argc < 4) show_route_usage() ;
			string prefix = argv[3] ;
			root["prefix"] = prefix ;
		}else if(cmd2 == "list"){

		}else{
			show_route_usage() ;
			exit(0) ;
		}
	}
	send_cmd(root.toStyledString()) ;
}

void face_cmd(int argc , char **argv){
	Json::Value root ;
	root["cmd1"]="face" ;
	if(argc == 2){
		root["cmd2"] = "list" ;
	}else{
		string cmd2 = argv[2] ;
		root["cmd2"] = cmd2;
		if(cmd2 == "add"){   // face add
			if(argc < 4) show_face_usage() ;
			string remote_url = argv[3] ;
			
			regex remoteurl_regex1(
					"tcp://([0-9]{1,3}\\.){3}[0-9]{1,3}");
			regex remoteurl_regex2(
					"ether://(([0-9]|[a-f]){2}:){5}([0-9]|[a-f]){2}");
			root["remote_url"] = remote_url ;
			if(regex_match(remote_url , remoteurl_regex1)){
				
			}else if(regex_match(remote_url, remoteurl_regex2)){
				if(argc < 5) show_face_usage() ;
				string local_dev = argv[4] ;
				regex localdev_regex("dev://.*");
				if(!regex_match(local_dev, localdev_regex)){
					show_face_usage();
					exit(0) ;
				}
				root["local_dev"] = local_dev ;
			}else{
				show_face_usage() ;
				exit(0) ;
			}
		}  // face add end
		else if(cmd2 == "remove"){
			if(argc < 4) show_face_usage() ;
			int face_id = atoi(argv[3]) ;
			root["face_id"] = face_id ;
		}else if(cmd2 == "list"){

		}else{
			show_face_usage() ;
			exit(0) ;
		}
	}
	send_cmd(root.toStyledString()) ;
}

void pit_cmd(int argc , char **argv){
	Json::Value root ;
	root["cmd1"]="pit" ;
	if(argc == 2){
		root["cmd2"] = "list" ;
	}else{
		show_usage() ;
	}
	send_cmd(root.toStyledString()) ;
}

int main(int argc , char **argv)
{
	if(argc < 2){
		show_usage() ;
	}
	string cmd = argv[1] ;
	if(cmd == "route"){
		route_cmd(argc,argv);
	}else if(cmd == "face"){
		face_cmd(argc,argv) ;
	}else if(cmd == "pit"){

	}
	else{
		show_usage() ;
	}

	return 0;
}
