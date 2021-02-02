// create by wgh
#ifndef _GLOBLE_VAR_H_
#define _GLOBLE_VAR_H_

#include <iostream>
#include <stdint.h>

using std::string ;

#define MAX_NAME_LEN 8000

extern uint16_t encode_name(char *buff , const string &name) ;
extern string decode_name(const char *name ) ;

#endif 
