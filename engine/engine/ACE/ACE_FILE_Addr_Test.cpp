#include "ACE_FILE_Addr_Test.h"
#include <iostream>
#include "ACE_Include.h"

void ACE_FILE_Addr_Test::write()
{
	ACE_FILE_Addr file_addr;
	file_addr.set("ACE_FILE_Addr_Test.log");
	ACE_FILE_IO new_io;
	ACE_FILE_Connector file_connector;
	if (-1 == file_connector.connect(new_io, file_addr, 0))
	{
		std::cout << "write() ACE_FILE_Connector::connect failed" << std::endl;
		return;
	}

	char buf[] = "hello ace_file_addr";
	size_t n = new_io.send_n(buf, sizeof(buf)-1);
	if (n <= 0)
	{
		std::cout << "write() ACE_FILE_IO::send_n failed" << std::endl;
	}
	new_io.close();
}
void ACE_FILE_Addr_Test::read()
{
	ACE_FILE_Addr file_addr;
	file_addr.set("ACE_FILE_Addr_Test.log");
	ACE_FILE_Connector file_connector;
	ACE_FILE_IO new_file_io;
	if (-1 == file_connector.connect(new_file_io, file_addr, 0))
	{
		std::cout << "read() ACE_FILE_Connector::connect failed" << std::endl;
	}
	char buf[100] = { 0 };
	size_t size = new_file_io.recv(buf, 100);
	if (size < 0)
	{
		std::cout << "read() ACE_FILE_IO::recv failed" << std::endl;
		new_file_io.close();
		return;
	}

	std::cout << buf << std::endl;
	new_file_io.close();
}

void ACE_FILE_Addr_Test::test(int argc, char ** argv)
{
	write();
	read();
}