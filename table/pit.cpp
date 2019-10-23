#include <iostream>
#include "pit.h"

using namespace std;

PIT *PIT::GetInstance(){
	if(m_pit == NULL){
		m_pit = new PIT();
	}
	return m_pit ;
}

PIT::PIT(){

}
PIT::~PIT(){

}

// param :
// name : TLV format [T:1|L:2|V]  V ::= [T:1|L:2|v]
void PIT::add(const char *name , const char *fhint ) {
	this->mpit_table.add(name,fhint);
}

// ret:
//		face list 
string PIT::search(const char *name ) {
	return this->mpit_table.search(name) ;
}

void PIT::remove(const char *name) {
	this->mpit_table.remove(name) ;
}

void PIT::update(){
	//cout << "update pit" << endl ;
}
