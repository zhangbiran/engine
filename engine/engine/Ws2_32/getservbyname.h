#ifndef __GET_SERV_BY_NAME_H
#define __GET_SERV_BY_NAME_H
#include "test.h"
#include <WinSock2.h>
#include <iostream>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

/*
C:\Windows\System32\drivers\etc\services

*/

/*
struct servent* FAR getservbyname(
_In_ const char *name,
_In_ const char *proto
);

*/

/*
typedef struct servent {
char FAR      *s_name;
char FAR  FAR **s_aliases;
short         s_port;
char FAR      *s_proto;
} SERVENT, *PSERVENT, FAR *LPSERVENT;

*/

class getservbynameTest : public CTest
{
public:
	void test(int argc, char ** argv)
	{
		WSAData data = { 0 };
		WSAStartup(MAKEWORD(2, 2), &data);

		struct servent * serv = getservbyname("netbios-ns", "udp");
		cout << serv->s_name << endl;
		for (char **pp = serv->s_aliases; *pp != NULL; pp++)
		{
			cout << *pp << endl;
		}

		cout << ntohs(serv->s_port) << endl;
		cout << serv->s_proto << endl;
		WSACleanup();
	}
};


#endif