#ifndef __GET_SERV_BY_PROT_H
#define __GET_SERV_BY_PROT_H
#include "test.h"
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
using namespace std;

/*
struct servent* FAR getservbyport(
_In_       int  port,
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


class getservbyportTest : public CTest
{
public:
	void test(int argc, char ** argv)
	{
		WSAData data = { 0 };
		WSAStartup(MAKEWORD(2, 2), &data);

		struct servent * serv = getservbyport(htons(666), "tcp");
		if (NULL == serv)
		{
			WSACleanup();
			return;
		}

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