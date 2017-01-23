#ifndef __TCP_SERVER_H
#define __TCP_SERVER_H

#include "test.h"


#include <Winsock2.h>
#include <windows.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
using namespace std;

/*
int setsockopt(
_In_       SOCKET s,
_In_       int    level,
_In_       int    optname,
_In_ const char   *optval,
_In_       int    optlen
);


*/


class tcp_server_Test : public CTest
{
public:
	void test(int argc, char **argv)
	{
		WSAData data = { 0 };
		WSAStartup(MAKEWORD(2, 2), &data);
		SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == sock)
		{
			cout << "socket error" << endl;
		}

		HOSTENT *localhost;
		localhost = gethostbyname("");
		char *ip = inet_ntoa(*(struct in_addr*)*localhost->h_addr_list);


		cout << ip << endl;
		struct sockaddr_in server_addr = { 0 };
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(7777);
		server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
		if (SOCKET_ERROR == bind(sock, (sockaddr*)& server_addr, sizeof(server_addr)))
		{
			cout << "bind error" << endl;
			closesocket(sock);
			WSACleanup();
		}

		
		if (SOCKET_ERROR == listen(sock, SOMAXCONN))
		{
			cout << "listen error" << endl;
			closesocket(sock);
			WSACleanup();
		}

		while (1)
		{
			SOCKET clientSock = INVALID_SOCKET;
			struct sockaddr_in clientAddr = { 0 };
			int len = sizeof(clientAddr);
			clientSock = accept(sock, (struct sockaddr*)&clientAddr, &len);
			if (INVALID_SOCKET == clientSock)
			{
				continue;
			}

			int size = 10;
			if (SOCKET_ERROR == setsockopt(clientSock, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(int)))
			{
				cout << "setsockopt failed" << ": " << WSAGetLastError() << endl;
				return;
			}

			int bufSize = 0;
			int sizeLen = sizeof(bufSize);
			if (SOCKET_ERROR != getsockopt(clientSock, SOL_SOCKET, SO_RCVBUF, (char*)&bufSize, &sizeLen))
			{
				cout << "recv buf :" << bufSize << endl;
			}

			while (1)
			{
				char buf[100] = { 0 };
				int n = recv(clientSock, buf, 20, 0);
				if (0 == n)
				{
					closesocket(clientSock);
					break;
				}
				else if (SOCKET_ERROR == n)
				{
					cout << "error" << endl;
					closesocket(clientSock);
					break;
				}
				else
				{
					cout << buf << " " << strlen(buf) << endl;
				}
			}
		}
	}
};

#endif