// create by wgh
#include <iostream>
#include "pit_map.h"

using namespace std ;

Pit_Map::Pit_Map(){

}

Pit_Map::~Pit_Map(){
	//map<string,struct pit_map_vt*>::iterator it = pit_map.begin() ;
	//for( ; it != pit_map.end() ; it++ ){
		//delete(it->second) ;
	//}
}

void Pit_Map::add(const char *name , const char* fhint){
	uint16_t name_len = *((uint16_t*)(name+1));
	string name_str(name,name_len+3);
	uint16_t fhint_len = *((uint16_t*)(fhint+1));
	string fhint_str(fhint, fhint_len+3) ;

	pit_map[name_str] = fhint_str ;

}

string Pit_Map::search(const char *name) {
	uint16_t name_len = *((uint16_t*)(name+1));
	string name_str(name,name_len+3);
	cout << "Pit_Map::search name_len= " << name_len << " " << name_str ; 
	if(name_len <= 0 ) exit(1) ;

	map<string, string>::iterator it = pit_map.find(name_str);
	if(it == pit_map.end()){
		// not existed
		string result="" ;
		return result ;
	}else{
		return it->second ;
	}
	
}

void Pit_Map::remove(const char *name){
	uint16_t name_len = *((uint16_t*)(name+1));
	string name_str(name,name_len+3);
	pit_map.erase(name_str) ;
}

void Pit_Map::update(){
}
