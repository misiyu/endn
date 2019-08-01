#include <iostream>

#include "../userapi/eface.h"

using namespace std;

static EFace eface ;
static int count = 0 ;
void onInterest(const EInterest& einterest){

	cout << "einterest count = "  << count << endl ;
	count ++ ;
	cout << "begin, onInterest<<<<<<<<<<<<" << endl ;

	cout << "einterest name = "<< einterest.getName() << endl ;
	cout << "source address  = "<< einterest.get_saddr() << endl ;
	//cout << "content = "<< einterest.getContent() << endl ;
	cout << "content len = "<< einterest.getContent().size() << endl ;

	cout << "end, onInterest<<<<<<<<<<<<" << endl ;
	
	string saddr = einterest.get_saddr() ;
	EData edata(saddr) ;
	string content = "hi-hi" ;
	edata.setContent(content.data() , content.size()) ;
	eface.put(edata) ;
	
}

int main()
{	// server
	string filter_name = "/daddr/002" ;  // 监听地址
	eface.setInterestFilter(filter_name , onInterest) ;
	eface.processEvents() ;		// 启动监听

	return 0;
}
