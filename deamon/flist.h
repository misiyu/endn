#ifndef _FLIST_H_
#define _FLIST_H_
#include "face/face.h"

#define FLIST_SZ 1000

class Face ;
class FList
{
public:
	~FList();
	static FList * GetInstance() ;
	int s_add_tcp_face(const char *cip, int sockfd);
	int c_add_tcp_face(const char *sip, int port );
	int get_new_fid();

	/* data */
	Face *flist[FLIST_SZ] ;
	int face_n ;
	
private :
	FList();
	static FList *m_flist ;
} ;

//FList *FList_obj ;
//void * void_gflist = (void*)&(FList_obj) ;

#endif 
