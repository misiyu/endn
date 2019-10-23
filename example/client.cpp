#include <iostream>
#include "../userapi/eface.h"
#include <sys/time.h>

using std::cout;
using std::endl;

static EFace eface ;
const int send_num = 500 ;
static int ondata_c = 0 ;
struct timeval start , end ;
void onData(const EData& edata){
	ondata_c ++ ;
	cout << "begin, onData <<<<<<<<<<<<<<<<<<<<<<<" << endl ;
	cout << "edata name = " << edata.getName() << endl ;
	cout << "edata content = " << edata.getContent() << endl ;
	cout << "end, onData <<<<<<<<<<<<<<<<<<<<<<<" << endl ;
	if(ondata_c >= send_num){
		eface.shutdown() ;
		gettimeofday(&end , NULL) ;
		long timeuse = 1000000*(end.tv_sec-start.tv_sec)+
			end.tv_usec-start.tv_usec ;
		double rate = 8000.0*send_num*8/1000000/(timeuse/1000000.0);
		printf("rate = %lf Mbps\n", rate) ;
	}
}

void *send_packet(void *param){
	EInterest *einterest = (EInterest*)param ;
	for (int i = 0; i < send_num; i++) {
		//usleep(10000) ;
		cout << "send_packet thread send " << i << endl ;
		eface.expressInterest(*einterest) ;
	}
}

int main()
{	// client
	gettimeofday(&start , NULL) ;
	string source_addr = "/saddr/001" ;    // 设置源地址
	string interest_name = "/daddr/002" ;	// 设置请求包名称
	eface.set_saddr(source_addr,onData) ;  // 设置face接收的包名称

	EInterest einterest(interest_name , source_addr) ;
	string content = "hello.";
	char content1[8000] ;
	memset(content1,'a' , 8000) ;
	//einterest.setContent(content.data(), content.size()) ;
	einterest.setContent(content1, 8000) ;
	eface.expressInterest(einterest);
	pthread_t tid ;
	pthread_create(&tid , NULL , send_packet , (void*)&einterest) ;
	eface.processEvents() ;		// 发送数据

	for (int i = 0; i < 10; i++) {
		eface.expressInterest(einterest) ;
	}

	return 0;
}
