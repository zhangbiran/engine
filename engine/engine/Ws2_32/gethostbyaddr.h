#ifndef __GET_HOST_BY_ADDR_H
#define __GET_HOST_BY_ADDR_H
#include "test.h"

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
using namespace std;


/*
C:\Windows\System32\drivers\etc\hosts

*/
/*
struct hostent* FAR gethostbyaddr(
_In_ const char *addr,
_In_       int  len,
_In_       int  type
);

*/


class gethostbyaddrTest : public CTest
{
public:
	void test(int argc, char ** argv)
	{
		WSAData data = { 0 };
		WSAStartup(MAKEWORD(2, 2), &data);
		struct in_addr addr = { 0 };
		addr.S_un.S_addr = inet_addr("65.55.39.10");//65.55.39.10 www.contoso.com //0.0.0.1±¾µØ
		if (INADDR_NONE == addr.S_un.S_addr)
		{
			cout << "inet_addr failed" << endl;
			WSACleanup();
			return;
		}

		struct hostent * remoteHost = gethostbyaddr((char*)&addr, 4, AF_INET);
		if (NULL == remoteHost)
		{
			WSACleanup();
			return;
		}

		cout << remoteHost->h_name << endl;
		char **pp = remoteHost->h_aliases;
		for (; *pp != NULL; pp++)
		{
			cout << *pp << endl;
		}
		switch (remoteHost->h_addrtype) {
		case AF_INET:
			printf("AF_INET\n");
			break;
		case AF_NETBIOS:
			printf("AF_NETBIOS\n");
			break;
		default:
			printf(" %d\n", remoteHost->h_addrtype);
			break;
		}

		cout << remoteHost->h_length << endl;
		for (char **ip = remoteHost->h_addr_list; *ip != NULL; ip++)
		{
			if (remoteHost->h_addrtype == AF_INET)
			{
				struct in_addr addr;
				addr.S_un.S_addr = *((unsigned long *)(*ip));
				cout << inet_ntoa(addr) << endl;
			}
			else if (remoteHost->h_addrtype == AF_INET6)
			{
				cout << "ip is AF_INET6" << endl;
			}
		}

		WSACleanup();
	}
};


#endif