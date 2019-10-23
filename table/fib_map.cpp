#include <iostream>
#include <sstream>
#include "fib_map.h"

using std::cout ;
using std::endl ;

Fib_Map::Fib_Map(){

}
Fib_Map::~Fib_Map(){

}

void Fib_Map::add(const char *name , int face_id){
	uint16_t name_len = *((uint16_t*)(name+1));
	string name_str(name,name_len+3);
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
// 参数：
//		name 名称 两层TLV格式 
//		s_len 前缀长度
//		len 名称长度
//	返回值 ： it 
//	
map<string, struct fib_map_vt*>::iterator Fib_Map::recur_search(const char *name ,
		int s_len , int len){
	uint16_t component_len = *((uint16_t*)(name+s_len+1));
	s_len += (component_len+3) ;
	if( s_len < len){
		map<string, struct fib_map_vt*>::iterator it = recur_search(
				name, s_len ,len);
		if(it != fib_map.end()) return it ;

		*((uint16_t*)(name+1)) = s_len-3 ;
		string name_str(name,s_len) ;
		//cout << "target = " << name_str << endl ;
		//for (int i = 0; i < s_len; i++) {
			//printf("%x ",name[i]) ;
		//}
		//printf("\n") ;
		return fib_map.find(name_str) ;
	}else{
		string name_str(name,s_len) ;
		//cout << "target = " << name_str << endl ;
		//for (int i = 0; i < s_len; i++) {
			//printf("%x ",name[i]) ;
		//}
		//printf("\n") ;
		return fib_map.find(name_str) ;
	}
}

vector<int> Fib_Map::search(const char *name) {
	uint16_t name_len = *((uint16_t*)(name+1));
	
	string target(name,name_len+3) ;
	//cout << "target = " << target << endl ;
	//cout << "table = "  << endl ;
	// probe ======================================================
	//map<string,struct fib_map_vt*>::iterator it1 = fib_map.begin() ;
	//for( ; it1 != fib_map.end() ; it1 ++ ){
		//cout << it1->first << endl ;
		//uint16_t n_len = *(uint16_t*)(it1->first.data()+1) + 3 ;
		//for (int i = 0; i < n_len; i++) {
			//printf("%x ",it1->first[i]) ;
		//}
		//printf("\n") ;
	//}
	// probe ======================================================

	map<string, struct fib_map_vt*>::iterator it = recur_search(name,3 ,
			name_len+3) ;
	if(it == fib_map.end()){
		// not existed
		vector<int> result ;
		return result ;
	}else{
		return it->second->face_list ;
	}
	
}

void Fib_Map::remove(const char *name){
	uint16_t name_len = *((uint16_t*)(name+1));
	string name_str(name+3,name_len);
	fib_map.erase(name_str) ;
}

string Fib_Map::get_table(){
	std::stringstream ss ;
	map<string,struct fib_map_vt*>::iterator it = fib_map.begin() ;
	for( ; it != fib_map.end() ; it ++ ){
		string prefix = decode_name(it->first.data()) ;
		ss << "prefix=" << prefix << " faceid=" ;
		vector<int> &face_list = it->second->face_list;
		for (int i = 0; i < face_list.size(); i++) {
			ss<<face_list[i]<<"," ;
		}
		ss << "\n" ;
	}
	return ss.str() ;
}
