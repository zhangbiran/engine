#ifndef __ACE_Thread_Manager_TEST_H
#define __ACE_Thread_Manager_TEST_H
#include "ACE_Include.h"
#include "test.h"

class ACE_Thread_Manager_Test : public CTest
{
public:
	struct Thread_Args
	{
		Thread_Args(ACE_Thread_Manager_Test * p) : _this(p) {}
		ACE_Thread_Manager_Test * _this;
	};
	static ACE_THR_FUNC_RETURN run_svc(void * args);
	void test(int argc, char **argv);
	void print();
};

#endif