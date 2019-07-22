#include <iostream>

#include "../userapi/eface.h"

using namespace std;

void onInterest(const EInterest& einterest){

}

int main()
{	// server
	string filter_name = "/daddr" ;  // 监听地址
	EFace eface ;
	eface.setInterestFilter(filter_name , onInterest) ;
	eface.processEvents() ;		// 启动监听

	return 0;
}
