// create by wgh
#ifndef _PIT_MAP_H_
#define _PIT_MAP_H_

#include <iostream>
#include <map>
#include <vector>

#define EXPIRE_T 3000   // expire time
using std::map ;
using std::pair ;
using std::vector ;
using std::string ;



struct pit_map_vt{
	vector<int> face_list ;
	int expire_time ;
	pit_map_vt(int face_id){
		face_list.push_back(face_id);
		expire_time = EXPIRE_T ;
	}
	void add_faceid(int face_id){
		expire_time = EXPIRE_T ;
		for (int i = 0; i < face_list.size(); i++) {
			if(face_list[i] == face_id) return ;
		}
		face_list.push_back(face_id) ;
	}
} ;

class Pit_Map
{
public:
	Pit_Map();
	~Pit_Map();
	// param :
	// name : TLV format [T:1|L:2|V]  V ::= [T:1|L:2|v]
	//void add(char *name , int face_id) ;
	void add(const char *name , const char* fhint) ;
	
	// ret:
	//		face list 
	string search(const char *name ) ;
	void remove(const char *name) ;
	void update();

private:
	map<string,string> pit_map ;
	/* data */
};

#endif 
