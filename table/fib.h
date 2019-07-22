#ifndef _FIB_H_
#define _FIB_H_

#include "fib_map.h"

class FIB
{
public:
	~FIB();
	static FIB *GetInstance() ;
	// param :
	// name : TLV format [T:1|L:2|V]  V ::= [T:1|L:2|v]
	void add(const char *name , int face_id) ;
	// ret:
	//		face list 
	vector<int> search(const char *name ) ;
	void remove(const char *name) ;
	void update() ;

private:
	FIB();
	/* data */
	Fib_Map mfib_table ; 
	static FIB *m_fib ;
};

#endif 
