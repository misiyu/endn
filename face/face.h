#ifndef _FACE_H_
#define _FACE_H_

#define RQueue (_this->mchannel->mrqueue)

#include <iostream>
#include "tcp_channel.h"
#include "common/globle_var.h"

using std::string ;

enum CH_TYPE { ETH , TCP } ;
enum FACE_STATE { ACTIVE , DEAD } ;

class FList ;
class Face
{
public:
	Face(const char *dip , int sockfd);
	void stop();
	void start() ;
	static void *search(void *param) ;
	int send2face(vector<int> &face_list, int &clen) ;
	int add2ch(char *data , int &len) ;

	enum FACE_STATE get_state(){
		return this->m_state ;
	}
	~Face() ;

private:
	/* data */
	int face_id ;
	int sockfd ;
	pthread_t mtid ;
	enum CH_TYPE m_ch_type ;
	enum FACE_STATE m_state ;
	Channel * mchannel ;
	string daddr ;
	FIB *m_fib ;
	PIT *m_pit ;
	FList *m_flist ;
	
};

#endif 
