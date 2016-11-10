#include <iostream>
#include "test.h"
#include "config.h"

int main(int argc, char **argv)
{
	CTest * test = CTestFactory::instance()->getTestObject(CConfig::instance()->getTestObjectType());
	if (test != NULL)
	{
		test->test(argc, argv);
	}
	std::cout << std::endl;
	return 0;
}