#ifndef __ACE_FILE_ADDR_TEST_H
#define __ACE_FILE_ADDR_TEST_H
#include "test/test.h"

class ACE_FILE_Addr_Test : public CTest
{
public:
	void write();
	void read();
	void test(int argc, char ** argv);
};



#endif