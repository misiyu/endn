// create by wgh
#include <iostream>

#include "../userapi/eface.h"
#include <sys/time.h>

using namespace std;

static EFace eface ;
static int count = 0 ;
struct timeval last ;
const int rate_val = 2000;
void onInterest(const EInterest& einterest){

	if(count == 0 ){
		gettimeofday(&last , NULL) ;
	}
	count ++ ;
	if(count % rate_val ==  0 ){
		struct timeval now ;
		gettimeofday(&now , NULL) ;
		long timeuse = 1000000*(now.tv_sec-last.tv_sec)+
			now.tv_usec-last.tv_usec ;
		double rate = 8.0*rate_val*8/1000/(timeuse/1000000.0);
		printf("rate = %lf Mbps\n", rate) ;
		gettimeofday(&last , NULL) ;
	}
	cout << "einterest count = "  << count << endl ;
	cout << einterest.getName() << endl ;
	
	string saddr = einterest.get_saddr() ;
	EData edata(saddr) ;
	string content = "hi-hi" ;
	edata.setContent(content.data() , content.size()) ;
	//cout << "probe 1 ===============" << endl ;
	eface.put(edata) ;
	//cout << "probe 2 ===============" << endl ;
	
}

int main()
{	// server
	memset(&last , 0 , sizeof(struct timeval)) ;
	string filter_name = "/daddr/002" ;  // 监听地址
	eface.setInterestFilter(filter_name , onInterest) ;
	eface.processEvents() ;		// 启动监听

	return 0;
}
