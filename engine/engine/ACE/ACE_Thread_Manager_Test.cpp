#include "ACE_Thread_Manager_Test.h"
#include <iostream>
using std::auto_ptr;

ACE_THR_FUNC_RETURN ACE_Thread_Manager_Test::run_svc(void * args)
{
	cout << "hello world" << endl;
	return 0;
}

void ACE_Thread_Manager_Test::test(int argc, char **argv)
{
	ACE_Thread_Manager mang;
	if (mang.spawn(
		ACE_Thread_Manager_Test::run_svc,
		NULL,
		THR_DETACHED | THR_SCOPE_SYSTEM) == -1)
	{
		cout << "spawn failed" << endl;
		return;
	}

	//ACE_Thread_Manager::instance()->wait();
}