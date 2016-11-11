#include "test/test.h"
#include "ACE_Test/ACE_Message_Block_Test.h"
#include "ACE_Test/ACE_FILE_Addr_Test.h"
#include "ACE_Test/ACE_Thread_Manager_Test.h"
#include "C++_Test/static_test.h"

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
	case TEST_TYPE_STATIC:
		return new StaticTest();
	case TEST_TYPE_ACE_FILE_Addr:
		return new ACE_FILE_Addr_Test();
	case TEST_TYPE_ACE_Thread_Manager:
		return new ACE_Thread_Manager_Test();
	default:
		break;
	}

	return NULL;
}

