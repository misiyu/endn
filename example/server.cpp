#include <iostream>

#include "../userapi/eface.h"
#include <sys/time.h>

using namespace std;

static EFace eface ;
static int count = 0 ;
struct timeval last ;
void onInterest(const EInterest& einterest){

	count ++ ;
	if(count % 2000 ==  0 ){
		struct timeval now ;
		gettimeofday(&now , NULL) ;
		long timeuse = 1000000*(now.tv_sec-last.tv_sec)+
			now.tv_usec-last.tv_usec ;
		double rate = 8000.0*count*8/1000000/(timeuse/1000000.0);
		printf("rate = %lf Mbps\n", rate) ;
		gettimeofday(&last , NULL) ;
	}
	cout << "einterest count = "  << count << endl ;
	//cout << "begin, onInterest<<<<<<<<<<<<" << endl ;
	//cout << "einterest name = "<< einterest.getName() << endl ;
	//cout << "source address  = "<< einterest.get_saddr() << endl ;
	////cout << "content = "<< einterest.getContent() << endl ;
	//cout << "content len = "<< einterest.getContent().size() << endl ;
	//cout << "end, onInterest<<<<<<<<<<<<" << endl ;
	
	string saddr = einterest.get_saddr() ;
	EData edata(saddr) ;
	string content = "hi-hi" ;
	edata.setContent(content.data() , content.size()) ;
	eface.put(edata) ;
	
}

int main()
{	// server
	memset(&last , 0 , sizeof(struct timeval)) ;
	string filter_name = "/daddr/002" ;  // 监听地址
	eface.setInterestFilter(filter_name , onInterest) ;
	eface.processEvents() ;		// 启动监听

	return 0;
}
