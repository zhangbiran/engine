#ifndef __UTILITY_FUNCTION_H
#define __UTILITY_FUNCTION_H
#include <windows.h>


void getLocalHostIp()
{
	HOSTENT * localhost = gethostbyname("");
	cout << localhost->h_name << endl;

	char *ip = inet_ntoa(*(struct in_addr*)localhost->h_addr_list[0]);
}


#endif