#ifndef __TEST_H
#define  __TEST_H

enum TEST_TYPE
{
	TEST_TYPE_ACE_MESSAGE_BLOCK = 1,	//ACE_Message_Block
	TEST_TYPE_STATIC = 2,				//static
	TEST_TYPE_ACE_FILE_Addr = 3,		//ACE_FILE_Addr
	TEST_TYPE_ACE_Thread_Manager = 4,	//ACE_Thread_Manager
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