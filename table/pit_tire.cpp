#include <iostream>
#include "pit_tire.h"

Pit_Tire::Pit_Tire(){
	this->root = new root(1,"/") ;
}

void delete_tire(struct pit_node_t* node_p){
	if(!(node_p->component)) free(node_p->component);
	if(!(node_p->full_name)) free(node_p->full_name) ;
	if(!(node_p->childs).empty()){
		for(forward_list<struct pit_node_t>::iterator it = 
				(node_p->childs).begin(); it != strs.end() ; it++){
			delete_tire(&(*it)) ;
		}
	}
}

Pit_Tire::~Pit_Tire(){
	if(root!=NULL){
		delete_tire(root) ;
	}
}

void insert_tire(struct pit_node_t* node_p , char *sub_name , int len 
		, int face_id){
	uint16_t clen = *((uint16_t*)(sub_name+1)) ;
	char *component = (sub_name+3) ;
	struct pit_node_t * next = NULL ;
	for(forward_list<struct pit_node_t>::iterator it = 
			(node_p->childs).begin(); it != strs.end() ; it++){
		if(clen != (node_p->len)) continue ;
		if(memcmp(component , (node_p->component) , clen) == 0){
			next = &(*it) ;
			break ;
		}
	}
	if(next){  // next != NULL
		if(clen+3 >= len){
			node_p->add_face(face_id) ;
		}else{
			node_p->is_leaf = 0 ;
			insert_tire(next , sub_name+clen+3 , len-clen-3 , face_id) ;
		}
	}else{
		struct pit_node_t* new_node = new pit_node_t(clen , component) ;
		node_p->add_node(*(new_node));
		if(clen+3 >= len) new_node->add_face(face_id);
		else insert_tire( new_node, sub_name+clen+3 , len-clen-3 , face_id) ;
	}
}

// param :
// char *name : is TLV format [1|2|[V]]
// int face_id : the forward face id for prefix "name"
Pit_Tire::insert(char *name ,int face_id){
	uint16_t nlen = *((uint16_t*)(name+1)) ;
	insert_tire(root , name+3 , nlen-3 , int face_id) ;
}

// 
// char *name : is TLV format [1|2|[V]]
// type : 05 request 
//		  06 response
void Pit_Tire::search(char *name , int type, forward_list<int> &face_list){
	
}
