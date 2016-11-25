#ifndef __SWAP_H
#define __SWAP_H

#include <iostream>
using namespace std;
#include "test.h"

class A
{
public:
	A(char *p) : _p(p) {}
	char *getP()
	{
		return _p;
	}
	A(const A& b)
	{
		_p = b._p;
	}
	A& operator = (const A& b)
	{
		_p = b._p;
		return *this;
	}
	void swap(A & b)
	{
		using std::swap;
		swap(_p, b._p);
	}
private:
	char * _p;
};

namespace std
{
	template <>			//ÌØ»¯
	void swap<A>(A& a, A& b)
	{
		cout << "swap" << endl;
		a.swap(b);
	}
}

class swapTest : public CTest
{
public:
	void test(int argc, char ** argv)
	{
		A a("hello A");
		A b("hello B");
		swap(a, b);
		/*
		A temp = a;
		a = b;
		b = temp;
		*/
		cout << a.getP() << endl;	//hello B
		cout << b.getP() << endl;	//hello A
	}
};

#endif