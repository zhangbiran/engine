#include "test.h"
#include "Test/ACE_Test/ACE_Message_Block_Test.h"
#include "Test/ACE_Test/ACE_FILE_Addr_Test.h"
#include "Test/ACE_Test/ACE_Thread_Manager_Test.h"
#include "Test/C++_Test/static_test.h"
#include "Test/C++_Test/swap.h"
#include "Test/VC_Test/NetWork/IOCP/IOCP_Server_Test.h"
#include "Test/VC_Test/NetWork/IOCP/IOCP_Client_Test.h"
#include "Test/VC_Test/NetWork/select/select_Server_Test.h"
#include "open_source/Json/Json_Test.h"
#include "open_source/TinyXml/TinyXml_Test.h"
#include "Thread/Win32/win32_thread.h"
#include "Event/Win32/win32_event.h"
#include "CriticalSection/CriticalSection.h"
#include "Mutex/Win32/Win32_Mutex.h"
#include "Semaphore/Win32/win32_semaphore.h"
#include "SynchronizationBarrier/SynchronizationBarrier.h"
#include "ConditionVariable/Win32/win32_ConditionVariable.h"
#include "APC/win32_apc.h"
#include "UDP/udp_client.h"
#include "UDP/udp_server.h"


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

	default:
		break;
	}

	return NULL;
}

