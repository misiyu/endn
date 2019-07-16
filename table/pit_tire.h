#ifndef _PIT_TIRE_H_
#define _PIT_TIRE_H_
#include <forward_list>

#define EXPIRE_T 300   // expire time

using std::forward_list ;

struct pit_node_t{
	int len ;
	char *component ;  // remember to free
	forward_list<struct pit_node_t> childs ;
	forward_list<int> face_list ; 
	int full_name_len ;
	char *full_name ;  // remember to free
	int expire_time ;  // 
	char is_leaf ;
	pit_node_t(int l , char *comp){
		component = (char*)malloc(sizeof(char)*l) ;
		memcpy(component,comp , l) ;
		len = l ;
		is_leaf = 0 ;
	}
	~pit_node_t(){
		if(component != NULL) free(component) ;
		if(full_name != NULL) free(full_name) ;
	}
	void add_face(int face_id){
		bool flag = true ;
		for(forward_list<int>::iterator it = face_list.begin() ;
				it != face_list.end() ; it++){
			if(face_id == (*it)) {
				flag = false ;
				break ;
			}
		}
		if(flag)  // face id not in face list
			face_list.push_front(face_id) ;
	}
	void add_node(struct pit_node_t new_node){
		childs.push_front(new_node) ;
	}
} ;

class Pit_Tire
{
public:
	Pit_Tire();
	~Pit_Tire();
	void insert(char *name, int face_id);
	void remove(char *name);
	void search(char *name , int type , forward_list<int> &face_list) ;
	// type = 05 request
	// type = 06 response

private:
	/* data */
	pit_node_t *root ;
	forward_list<struct pit_node_t*> leaf_list;
};

#endif 
