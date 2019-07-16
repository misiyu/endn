#ifndef _TIRE_H_
#define _TIRE_H_
#include <forward_list>

#define T_Tree_MFork 100

using std::forward_list ;

struct fib_node_t{
	int len ;
	char *component ;
	forward_list<struct t_node_t> childs;
	forward_list<int> face_list ; 
};

struct pit_node_t{
	int len ;
	char *component ;
	forward_list<struct pit_node_t> childs ;
	forward_list<int> face_list ; 
	int expire_time ;

}

#endif 
