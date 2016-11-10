#include "test.h"
#include "ACE_Message_Block_Test.h"
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
	case TEST_TYPE_ACE_MESSAGE_BLOCK_TEST:
	{
		return new ACE_Message_Block_Test();
	}
	default:
		break;
	}

	return NULL;
}

