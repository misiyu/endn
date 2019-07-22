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
	void add(char *name , int face_id) ;
	
	// ret:
	//		face list 
	vector<int> search(char *name ) ;

	void remove(char *name) ;
	void update();

private:
	PIT();
	/* data */
	Pit_Map mpit_table ;
	static PIT *m_pit ;

};

#endif 
