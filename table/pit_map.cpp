#include <iostream>
#include "pit_map.h"

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
	string name_str(name+3,name_len);
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
	string name_str(name+3,name_len);
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
	string name_str(name+3,name_len);
	pit_map.erase(name_str) ;
}

void Pit_Map::update(){
	map<string,struct pit_map_vt*>::iterator it = pit_map.begin() ;
	for( ; it != pit_map.end() ; it++ ){
		it->second->expire_time -- ;
		if(it->second->expire_time <= 0) pit_map.erase(it);
	}
}
