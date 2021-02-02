// create by wgh
#ifndef _PIT_H_
#define _PIT_H_

#include "pit_map.h"

class PIT
{
public:
	~PIT();
	static PIT *GetInstance() ;

	// param :
	// name : TLV format [T:1|L:2|V]  V ::= [T:1|L:2|v]
	void add(const char *name , const char* fhint) ;
	
	// ret:
	//		face list 
	string search(const char *name ) ;

	void remove(const char *name) ;
	void update();

private:
	PIT();
	/* data */
	Pit_Map mpit_table ;
	static PIT *m_pit ;

};

#endif 
