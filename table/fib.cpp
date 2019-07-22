#include <iostream>
#include "fib.h"

using namespace std;

FIB *FIB::GetInstance(){
	if(m_fib == NULL){
		m_fib = new FIB();
	}
	return m_fib ;
}

FIB::FIB(){

}

FIB::~FIB(){

}


// param :
// name : TLV format [T:1|L:2|V]  V ::= [T:1|L:2|v]
void FIB::add(const char *name , int face_id) {
	mfib_table.add(name,face_id);
}

// ret:
//		face list 
vector<int> FIB::search(const char *name ) {
	return mfib_table.search(name) ;
}

void FIB::remove(const char *name) {
	mfib_table.remove(name) ;
}

void FIB::update(){
	//cout << "update fib " << endl ;
}
