#ifndef __GET_PEER_NAME_H
#define __GET_PEER_NAME_H
#include "test.h"
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
using namespace std;

class getpeernameTest : public CTest
{
public:
	void test(int argc, char ** argv)
	{
		WSAData data = { 0 };
		WSAStartup(MAKEWORD(2, 2), &data);

		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (INVALID_SOCKET == sock)
		{
			return;
		}

		struct sockaddr_in serverAddr = { 0 };
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(7777);
		serverAddr.sin_addr.S_un.S_addr = inet_addr("192.168.9.202");
		if (INVALID_SOCKET == connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)))
		{
			return;
		}

		send(sock, "hello", strlen("hello"), 0);

		struct sockaddr_in peerAddr = { 0 };
		int len = sizeof(peerAddr);
		if (0 != getpeername(sock, (struct sockaddr*)&peerAddr, &len))
		{
			cout << "getpeername failed" << WSAGetLastError() << endl;
			return;
		}

		cout << ntohs(peerAddr.sin_port) << endl;
		cout << inet_ntoa(peerAddr.sin_addr) << endl;
	}
};


#endif