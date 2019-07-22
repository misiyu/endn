#ifndef _ENAME_H_
#define _ENAME_H_

#include <iostream>
using std::string ;

class EName
{
public:
	EName(const char *name);
	~EName();
	string getData() ;

private:
	/* data */
	string name ;
};

#endif 
