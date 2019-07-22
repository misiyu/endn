#include <iostream>
#include "ename.h"

using namespace std;

EName::EName(const char *name){
	this->name = name ;
}
EName::~EName(){

}

string EName::getData(){
	return name ;
}
