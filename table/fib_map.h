#ifndef _FIB_MAP_H_
#define _FIB_MAP_H_

#include <iostream>
#include <map>
#include <vector>

using std::map ;
using std::pair ;
using std::vector ;
using std::string ;

#include "common/globle_var.h"

#define EXPIRE_T 300   // expire time s

struct fib_map_vt{
	vector<int> face_list ;
	fib_map_vt(int face_id){
		face_list.push_back(face_id);
	}
	void add_faceid(int face_id){
		for (int i = 0; i < face_list.size(); i++) {
			if(face_list[i] == face_id) return ;
		}
		face_list.push_back(face_id) ;
	}
};

class Fib_Map
{
public:
	Fib_Map();
	~Fib_Map();

	// param :
	// name : TLV format [T:1|L:2|V]  V ::= [T:1|L:2|v]
	void add(const char *name , int face_id) ;
	
	// ret:
	//		face list 
	vector<int> search(const char *name ) ;

	void remove(const char *name) ;
	map<string, struct fib_map_vt*>::iterator recur_search(const char *name ,
		int s_len , int len);
	string get_table();

private:
	map<string,struct fib_map_vt*> fib_map ;
	/* data */
};

#endif 
