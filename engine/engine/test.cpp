#include "test.h"
#include "ACE/ACE_Message_Block_Test.h"
#include "ACE/ACE_FILE_Addr_Test.h"
#include "ACE/ACE_Thread_Manager_Test.h"
#include "C++/static_test.h"
#include "C++/swap.h"
#include "VC++/NetWork/IOCP/IOCP_Server_Test.h"
#include "VC++/NetWork/IOCP/IOCP_Client_Test.h"
#include "VC++/NetWork/select/select_Server_Test.h"
#include "open_source/Json/Json_Test.h"
#include "open_source/TinyXml/TinyXml_Test.h"
#include "Thread/Win32/win32_thread.h"
#include "synchronization/Event/Win32/win32_event.h"
#include "synchronization/CriticalSection/CriticalSection.h"
#include "synchronization/Mutex/Win32/Win32_Mutex.h"
#include "synchronization/Semaphore/Win32/win32_semaphore.h"
#include "synchronization/SynchronizationBarrier/SynchronizationBarrier.h"
#include "synchronization/ConditionVariable/Win32/win32_ConditionVariable.h"
#include "synchronization/APC/win32_apc.h"
#include "NetWork/UDP/udp_client.h"
#include "NetWork/UDP/udp_server.h"
#include "NetWork/TCP/tcp_server.h"
#include "Ws2_32/gethostbyname.h"
#include "Ws2_32/gethostbyaddr.h"
#include "Ws2_32/getprotobyname.h"
#include "Ws2_32/getservbyname.h"
#include "Ws2_32/getservbyport.h"
#include "Ws2_32/getprotobynumber.h"
#include "Ws2_32/getnameinfo.h"
#include "Ws2_32/getpeername.h"



#include <iostream>


CTestFactory* CTestFactory::instance()
{
	static CTestFactory obj;
	return &obj;
}
CTest * CTestFactory::getTestObject(int type)
{
	switch (type)
	{
	case TEST_TYPE_ACE_MESSAGE_BLOCK:
		return new ACE_Message_Block_Test();
	case TEST_TYPE_STD_STATIC:
		return new Static_Test();
	case TEST_TYPE_ACE_FILE_Addr:
		return new ACE_FILE_Addr_Test();
	case TEST_TYPE_ACE_Thread_Manager:
		return new ACE_Thread_Manager_Test();
	case TEST_TYPE_IOCP_SERVER:
		return new IOCP_Server_Test();
	case TEST_TYPE_IOCP_CLIENT:
		return new IOCP_Client_Test();
	case TEST_TYPE_SELECT_SERVER:
		return new Select_Server_Test();
	case TEST_TYPE_JSON:
		return new Json_Test();
	case TEST_TYPE_TINYXML:
		return new TinyXml_Test();
	case TEST_TYPE_WIN32_THREAD:
		return new Win32_Therad();
	case TEST_TYPE_WIN32_EVENT:
		return new Win32_Event();
	case TEST_TYPE_WIN32_CRITICAL_SECTION:
		return new CriticalSection();
	case TEST_TYPE_WIN32_MUTEX:
		return new Win32_Mutex();
	case TEST_TYPE_WIN32_SEMAPHORE:
		return new Win32_Semaphore();
	case TEST_TYPE_WIN32_SYNTHRONIZATION_BARRIER:
		return new SynchronizationBarrier();
	case TEST_TYPE_WIN32_CONDITION_VARIABLE:
		return new Win32ConditionVariable();
	case TEYP_TYPE_WIN32_APC:
		return new Win32_APC();
	case TEST_TYPE_STD_SWAP:
		return new swapTest();
	case TEST_TYPE_NETWORK_UDP_CLIENT:
		return new UDP_client();
	case TEST_TYPE_NETWORK_UDP_SERVER:
		return new UDP_server();
	case TEST_TYPE_WS2_32_GET_HOST_BY_NAME:
		return new gethostbynameTest();
	case TEST_TYPE_WS2_32_GET_HOST_BY_ADDR:
		return new gethostbyaddrTest();
	case TEST_TYPE_WS2_32_GET_PROTO_BY_NAME:
		return new getprotobynameTest();
	case TEST_TYPE_WS2_32_GET_SERV_BY_NAME:
		return new getservbynameTest();
	case TEST_TYPE_WS2_32_GET_SERV_BY_PORT:
		return new getservbyportTest();
	case TEST_TYPE_WS2_32_GET_PROTO_BY_NUMBER:
		return new getprotobynumberTest();
	case TEST_TYPE_WS2_32_GET_NAME_INFO:
		return new getnameinfoTest();
	case TEST_TYPE_WS2_32_GET_PEER_NAME:
		return new getpeernameTest();
	case TEST_TYPE_NETWORK_TCP_SERVER:
		return new tcp_server_Test();


	default:
		break;
	}

	return NULL;
}

