#include <iostream>

#include "deamon/endn_deamon.h"
FIB * FIB::m_fib = NULL ;   // 初始化静态成员
PIT * PIT::m_pit = NULL ;
FList * FList::m_flist = NULL ;
int main()
{
	//printf("main.cpp gflist = %p\n" ,gflist) ;
	FIB *fib = FIB::GetInstance() ;
	PIT *pit = PIT::GetInstance() ;
	FList *flist = FList::GetInstance() ;
	
	Endn_Deamon endnD ;
	endnD.start() ;

	delete(fib) ;
	delete(pit) ;
	delete(flist) ;

	return 0;
}
