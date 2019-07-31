#include <iostream>
#include "../userapi/eface.h"

using namespace std;

static EFace eface ;
void onData(const EData& edata){

	cout << "begin, onData <<<<<<<<<<<<<<<<<<<<<<<" << endl ;
	cout << "edata name = " << edata.getName() << endl ;
	cout << "edata content = " << edata.getContent() << endl ;
	cout << "end, onData <<<<<<<<<<<<<<<<<<<<<<<" << endl ;
}

void *send_packet(void *param){
	EInterest *einterest = (EInterest*)param ;
	for (int i = 0; i < 10000; i++) {
		//usleep(10000) ;
		cout << "send_packet thread send " << i << endl ;
		eface.expressInterest(*einterest) ;
	}
}

int main()
{	// client
	string source_addr = "/saddr/001" ;    // 设置源地址
	string interest_name = "/daddr/002" ;	// 设置请求包名称
	eface.set_saddr(source_addr,onData) ;  // 设置face接收的包名称

	EName ename(interest_name.data()) ;
	EInterest einterest(ename , source_addr) ;
	string content = "hello.";
	einterest.setContent(content.data(), content.size()) ;
	//for (int i = 0; i < 10; i++) {
		eface.expressInterest(einterest);
	//}
	pthread_t tid ;
	pthread_create(&tid , NULL , send_packet , (void*)&einterest) ;
	eface.processEvents() ;		// 发送数据

	for (int i = 0; i < 10; i++) {
		eface.expressInterest(einterest) ;
	}

	return 0;
}
