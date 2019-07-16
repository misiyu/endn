#ifndef _FIB_H_
#define _FIB_H_

#include "fib_map.h"

class FIB
{
public:
	FIB();
	~FIB();
	// param :
	// name : TLV format [T:1|L:2|V]  V ::= [T:1|L:2|v]
	void add(char *name , int face_id) ;
	
	// ret:
	//		face list 
	vector<int> search(char *name ) ;

	void remove(char *name) ;

private:
	/* data */

};

#endif 
