#ifndef __WINDOWS_NETWORK_TEST_H
#define __WINDOWS_NETWORK_TEST_H

#include "nbcommon.h"
#include <iostream>
using namespace std;

int LanaEnum_test()
{
	LANA_ENUM lana;
	ZeroMemory(&lana, sizeof(LANA_ENUM));
	if (NRC_GOODRET != LanaEnum(&lana))
	{
		return -1;
	}

	for (int i = 0; i < lana.length; ++i)
	{
		printf("%d ", lana.lana[i]);
	}
	cout << endl;

	return 0;
}



#endif