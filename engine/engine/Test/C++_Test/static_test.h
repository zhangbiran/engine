#ifndef __STATIC_TEST_H
#define __STATIC_TEST_H
#include "test.h"


class CStatic_A
{
public:
	static CStatic_A * instance();
	virtual void print() = 0;
protected:
	CStatic_A() {}
	static CStatic_A * _ps;
};

class CStatic_B : public CStatic_A
{
public:
	void print();
};

class Static_Test : public CTest
{
public:
	void test(int argc, char ** argv);
};

#endif