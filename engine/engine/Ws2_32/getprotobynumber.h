#ifndef __GET_PROTO_BY_NUMBER_H
#define __GET_PROTO_BY_NUMBER_H

#include "test.h"
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

/*
struct PROTOENT* FAR getprotobynumber(
_In_ int number
);

*/

/*
typedef struct protoent {
char FAR      *p_name;
char FAR  FAR **p_aliases;
short         p_proto;
} protoent;
*/

class getprotobynumberTest : public CTest
{
public:
	void test(int argc, char** argv)
	{

		WSAData data = { 0 };
		WSAStartup(MAKEWORD(2, 2), &data);

		struct protoent * proto = getprotobynumber(6);	//6 tcp  udp 17
		if (NULL == proto)
		{
			WSACleanup();
			return;
		}

		cout << proto->p_name << endl;
		for (char** pp = proto->p_aliases; *pp != NULL; pp++)
		{
			cout << *pp << endl;
		}
		cout << proto->p_proto << endl;
		WSACleanup();
	}
};

#endif