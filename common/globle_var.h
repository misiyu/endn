#ifndef _GLOBLE_VAR_H_
#define _GLOBLE_VAR_H_

#include "table/fib.h"
#include "table/pit.h"
//#include "face/face.h"
#include "deamon/flist.h"

#define MAX_NAME_LEN 8000

//FIB gfib ;
//PIT gpit ;
extern void *void_gflist ;  
// this point will point to a FList object defined beblow some where

#define gflist ((FList*)void_gflist)

#endif 
