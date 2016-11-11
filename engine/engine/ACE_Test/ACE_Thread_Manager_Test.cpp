#include "ACE_Thread_Manager_Test.h"
#include <iostream>
using std::auto_ptr;

ACE_THR_FUNC_RETURN ACE_Thread_Manager_Test::run_svc(void * args)
{
	ACE_FILE_Addr file_addr;
	file_addr.set("ACE_FILE_Addr_Test111111111.log");
	ACE_FILE_IO new_io;
	ACE_FILE_Connector file_connector;
	if (-1 == file_connector.connect(new_io, file_addr, 0))
	{
		std::cout << "write() ACE_FILE_Connector::connect failed" << std::endl;
		return 0;
	}

	char buf[] = "hello ace_file_addr";
	size_t n = new_io.send_n(buf, sizeof(buf)-1);
	if (n <= 0)
	{
		std::cout << "write() ACE_FILE_IO::send_n failed" << std::endl;
	}
	new_io.close();

	std::cout << "run_svc" << std::endl;
	auto_ptr<Thread_Args> thread_args(static_cast<Thread_Args*>(args));
	thread_args->_this->print();
	return 0;
}

void ACE_Thread_Manager_Test::test(int argc, char **argv)
{
	printf("hello thread\n");
	auto_ptr<Thread_Args> thread_args(new Thread_Args(this));
	for (int i = 0; i < 4; i++)
	{
		if (ACE_Thread_Manager::instance()->spawn(
			// Pointer to function entry point.
			ACE_Thread_Manager_Test::run_svc,
			// <run_svc> parameter.
			static_cast<void *> (thread_args.get()),
			THR_DETACHED | THR_SCOPE_SYSTEM) == -1)
		{
			std::cout << "create thread failed" << std::endl;
		}
		std::cout << "success " << i << std::endl;
		ACE_Thread_Manager::instance()->wait();
	}
	
	std::cout << "all thread end" << std::endl;
}

void ACE_Thread_Manager_Test::print()
{
	static int i = 0;
	std::cout << i << std::endl;
}