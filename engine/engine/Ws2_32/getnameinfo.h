#ifndef __GET_NAME_INFO_H
#define __GET_NAME_INFO_H

#include "test.h"
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
using namespace std;

/*
int WSAAPI getnameinfo(
_In_  const struct sockaddr FAR *sa,
_In_  socklen_t                 salen,
_Out_ char FAR                  *host,
_In_  DWORD                     hostlen,
_Out_ char FAR                  *serv,
_In_  DWORD                     servlen,
_In_  int                       flags
);

*/

class getnameinfoTest : public CTest
{
public:
	void test(int argc, char** argv)
	{
		WSAData data = { 0 };
		WSAStartup(MAKEWORD(2, 2), &data);
		struct sockaddr_in addr = { 0 };
		addr.sin_family = AF_INET;
		addr.sin_port = htons(7777);
		addr.sin_addr.S_un.S_addr = inet_addr("192.168.9.85");

		char hostBuf[100] = { 0 };
		char serverBuf[100] = { 0 };
		if (0 != getnameinfo((struct sockaddr*)&addr, sizeof(addr), hostBuf, 100, serverBuf, 100, NI_NUMERICSERV))
		{
			cout << "getnameinfo failed" << endl;
			WSACleanup();
			return;
		}
		
		cout << hostBuf << endl;	//192.168.9.82的主机名
		cout << serverBuf << endl;	//以端口号字符串的服务名

		WSACleanup();
	}
};



#endif