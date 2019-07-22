#include <iostream>
#include "../userapi/eface.h"

using namespace std;

void onData(const EData& edata){

}

int main()
{	// client
	string source_addr = "/saddr/001" ;    // 设置源地址
	string interest_name = "/daddr/002/aaa" ;	// 设置请求包名称
	EFace eface ;
	eface.set_saddr(source_addr,onData) ;
	EName ename(interest_name.data()) ;
	EInterest einterest(ename , source_addr) ;
	eface.expressInterest(einterest);
	eface.processEvents() ;		// 发送数据

	return 0;
}
