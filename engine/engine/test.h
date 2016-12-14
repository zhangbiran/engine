#ifndef __TEST_H
#define  __TEST_H

#include <iostream>
using namespace std;

enum TEST_TYPE
{
	TEST_TYPE_ACE_MESSAGE_BLOCK = 1,	//ACE_Message_Block
	TEST_TYPE_STD_STATIC = 2,			//std static
	TEST_TYPE_ACE_FILE_Addr = 3,		//ACE_FILE_Addr
	TEST_TYPE_ACE_Thread_Manager = 4,	//ACE_Thread_Manager
	TEST_TYPE_IOCP_SERVER = 5,			//iocp_server
	TEST_TYPE_IOCP_CLIENT = 6,			//iocp_client
	TEST_TYPE_SELECT_SERVER = 7,		//select_server
	TEST_TYPE_JSON = 8,					//Json
	TEST_TYPE_TINYXML = 9,				//tinyxml
	TEST_TYPE_WIN32_THREAD = 10,		//win32 thread
	TEST_TYPE_WIN32_EVENT = 11,			//win32 event
	TEST_TYPE_WIN32_CRITICAL_SECTION = 12, //win32 critical section
	TEST_TYPE_WIN32_MUTEX = 13,			//win32 mutex
	TEST_TYPE_WIN32_SEMAPHORE = 14,		//win32 semaphore
	TEST_TYPE_WIN32_SYNTHRONIZATION_BARRIER = 15, //win32 synchronization barrier
	TEST_TYPE_WIN32_CONDITION_VARIABLE = 16, //win32 condition variable
	TEYP_TYPE_WIN32_APC = 17,			//win32 Asynchronous Procedure Call(异步过程调用)
	TEST_TYPE_STD_SWAP = 18,			//std swap
	TEST_TYPE_NETWORK_UDP_SERVER = 19,	//network udp server
	TEST_TYPE_NETWORK_UDP_CLIENT = 20,	//network udp client
	TEST_TYPE_WS2_32_GET_HOST_BY_NAME = 21, //ws2_32	gethostbyname
	TEST_TYPE_WS2_32_GET_HOST_BY_ADDR = 22, //ws2_32	gethostbyaddr
	TEST_TYPE_WS2_32_GET_PROTO_BY_NAME = 23, // ws2_32	getprotobyname
	TEST_TYPE_WS2_32_GET_SERV_BY_NAME = 24,	//ws2_32	getservbyname
	TEST_TYPE_WS2_32_GET_SERV_BY_PORT = 25, //ws2_32	getservbyport
	TEST_TYPE_WS2_32_GET_PROTO_BY_NUMBER = 26, //ws2_32 getprotobynumber
	TEST_TYPE_WS2_32_GET_NAME_INFO = 27,	//ws2_32	getnameinfo
	TEST_TYPE_WS2_32_GET_PEER_NAME = 28,	//ws2_32	getpeername
	TEST_TYPE_NETWORK_TCP_SERVER = 29,		//network	TCP server
};

class CTest
{
public:
	virtual void test(int argc, char **argv) = 0;
};

class CTestFactory
{
public:
	static CTestFactory* instance();
	CTest *getTestObject(int seq);
private:
	CTestFactory() {}
};



#endif