#include "select_Server_Test.h"
#include <windows.h>
#include <iostream>
using namespace std;

#pragma comment(lib,"Ws2_32.lib")

void Select_Server_Test::test(int argc, char ** argv)
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
	server_addr.sin_addr.s_addr = inet_addr(ip);
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

	fd_set readSock, allSock;
	FD_ZERO(&readSock);
	FD_ZERO(&allSock);
	FD_SET(sock, &allSock);
	int num = 1;

	while (1)
	{
		readSock = allSock;
		cout << "select ..." << endl;
		int ret = select(0, &readSock, NULL, NULL, NULL);
		cout << "event occur" << endl;
		if (SOCKET_ERROR == ret)
		{
			cout << "select error" << endl;
			break;
		}
		for (int i = 0; i < num; i++)
		{
			if (sock == readSock.fd_array[i])
			{
				sockaddr_in client_addr = { 0 };
				int len = sizeof(client_addr);
				SOCKET sock2 = accept(sock, (sockaddr*)&client_addr, &len);
				if (INVALID_SOCKET == sock2)
				{
					cout << "accept error" << endl;
					exit(-1);
				}
				cout << "client ip :" << inet_ntoa(client_addr.sin_addr) << endl;
				FD_SET(sock2, &allSock);
			}
			else
			{
				char buf[100] = { 0 };
				int ret = recv(readSock.fd_array[i], buf, 100, 0);
				if (SOCKET_ERROR == ret)
				{
					cout << "recv error" << endl;
					FD_CLR(readSock.fd_array[i], &allSock);
				}
				if (ret > 0)
				{
					cout << buf << endl;
				}
			}
		}
	}

	WSACleanup();
}
