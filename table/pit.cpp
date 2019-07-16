#include <iostream>
#include "pit.h"

using namespace std;

PIT::PIT(){

}
PIT::~PIT(){

}

// param :
// name : TLV format [T:1|L:2|V]  V ::= [T:1|L:2|v]
void PIT::add(char *name , int face_id) {
	this->mpit_table.add(name,face_id);
}

// ret:
//		face list 
vector<int> PIT::search(char *name ) {
	return this->mpit_table.search(name) ;
}

void PIT::remove(char *name) {
	this->mpit_table.remove(name) ;
}
