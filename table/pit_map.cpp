#include <iostream>
#include "pit_map.h"

using namespace std ;

Pit_Map::Pit_Map(){

}

Pit_Map::~Pit_Map(){
	map<string,struct pit_map_vt*>::iterator it = pit_map.begin() ;
	for( ; it != pit_map.end() ; it++ ){
		delete(it->second) ;
	}
}

void Pit_Map::add(char *name , int face_id){
	uint16_t name_len = *((uint16_t*)(name+1));
	string name_str(name,name_len+3);
	map<string, struct pit_map_vt*>::iterator it = pit_map.find(name_str);
	if(it == pit_map.end()){
		// not existed
		struct pit_map_vt *new_r = new pit_map_vt(face_id);
		pit_map.insert(pair<string,struct pit_map_vt*>(name_str,new_r)) ;
	}else{
		it->second->add_faceid(face_id) ;
	}
}

vector<int> Pit_Map::search(char *name) {
	uint16_t name_len = *((uint16_t*)(name+1));
	string name_str(name,name_len+3);

	// probe ======================================================
	//cout << "target = " << name_str << endl ;
	//map<string,struct pit_map_vt*>::iterator it1 = pit_map.begin() ;
	//for( ; it1 != pit_map.end() ; it1 ++ ){
		//cout << it1->first << endl ;
		//uint16_t n_len = *(uint16_t*)(it1->first.data()+1) + 3 ;
		//for (int i = 0; i < n_len; i++) {
			//printf("%x ",it1->first[i]) ;
		//}
		//printf("\n") ;
	//}
	// probe ======================================================

	map<string, struct pit_map_vt*>::iterator it = pit_map.find(name_str);
	if(it == pit_map.end()){
		// not existed
		vector<int> result ;
		return result ;
	}else{
		return it->second->face_list ;
	}
	
}

void Pit_Map::remove(char *name){
	uint16_t name_len = *((uint16_t*)(name+1));
	string name_str(name,name_len+3);
	pit_map.erase(name_str) ;
}

void Pit_Map::update(){
	map<string,struct pit_map_vt*>::iterator it = pit_map.begin() ;
	for( ; it != pit_map.end() ; it++ ){
		it->second->expire_time -- ;
		if(it->second->expire_time <= 0) pit_map.erase(it);
	}
}
