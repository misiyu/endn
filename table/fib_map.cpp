#include <iostream>
#include "fib_map.h"

Fib_Map::Fib_Map(){

}
Fib_Map::~Fib_Map(){

}

void Fib_Map::add(char *name , int face_id){
	uint16_t name_len = *((uint16_t*)(name+1));
	string name_str(name+3,name_len);
	map<string, struct fib_map_vt*>::iterator it = fib_map.find(name_str);
	if(it == fib_map.end()){
		// not existed
		struct fib_map_vt *new_r = new fib_map_vt(face_id);
		fib_map.insert(pair<string,struct fib_map_vt*>(name_str,new_r)) ;
	}else{
		it->second->add_faceid(face_id) ;
	}
}

// 递归查找，实现最长前缀匹配
map<string, struct fib_map_vt*>::iterator Fib_Map::recur_search(char *name ,
		int s_len , int len){
	uint16_t component_len = *((uint16_t*)(name+1));
	s_len += (component_len+3) ;
	if( s_len < len){
		map<string, struct fib_map_vt*>::iterator it = recur_search(name,s_len
				,len);
		if(it != fib_map.end()) return it ;
		string name_str(name,s_len) ;
		return fib_map.find(name_str) ;
	}else{
		string name_str(name,s_len) ;
		return fib_map.find(name_str) ;
	}
}

vector<int> Fib_Map::search(char *name) {
	uint16_t name_len = *((uint16_t*)(name+1));
	map<string, struct fib_map_vt*>::iterator it = recur_search(name+3,0 ,
			name_len) ;
	if(it == fib_map.end()){
		// not existed
		vector<int> result ;
		return result ;
	}else{
		return it->second->face_list ;
	}
	
}

void Fib_Map::remove(char *name){
	uint16_t name_len = *((uint16_t*)(name+1));
	string name_str(name+3,name_len);
	fib_map.erase(name_str) ;
}
