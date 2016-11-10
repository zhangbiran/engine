#ifndef __TEST_H
#define  __TEST_H

enum TEST_TYPE
{
	TEST_TYPE_ACE_MESSAGE_BLOCK_TEST = 1,	//ACE_Message_Block
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