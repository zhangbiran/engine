#ifndef __GET_PROTO_BY_NAME_H
#define __GET_PROTO_BY_NAME_H
#include "test.h"
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
using namespace std;

/*
C:\Windows\System32\drivers\etc\protocol


*/
/*
struct PROTOENT* FAR getprotobyname(
_In_ const char *name
);
*/


/*
typedef struct protoent {
char FAR      *p_name;
char FAR  FAR **p_aliases;
short         p_proto;
} protoent;


*/

class getprotobynameTest : public CTest
{
public:
	void test(int argc, char ** argv)
	{
		WSAData data = { 0 };
		WSAStartup(MAKEWORD(2, 2), &data);

		struct protoent * proto = getprotobyname("tcp");
		if (NULL == proto)
		{
			WSACleanup();
			return;
		}

		cout << proto->p_name << endl;
		cout << proto->p_proto << endl;
		for (char **pp = proto->p_aliases; *pp != NULL; pp++)
		{
			cout << *pp << endl;
		}
		WSACleanup();
	}
};


#endif