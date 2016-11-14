#include "static_test.h"
#include <iostream>


CStatic_A * CStatic_A::_ps = NULL;

CStatic_A * CStatic_A::instance()
{
	static CStatic_B obj;
	_ps = &obj;
	return _ps;
}

void CStatic_B::print()
{
	std::cout << "CStatic_B::print()" << std::endl;
}

void Static_Test::test(int argc, char ** argv)
{
	CStatic_A *p = CStatic_A::instance();
	if (p != NULL)
	{
		p->print();
	}
}
