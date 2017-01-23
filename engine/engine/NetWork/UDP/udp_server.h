#ifndef __UDP_SERVER_H
#define __UDP_SERVER_H

#include "test.h"
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
using namespace std;

class UDP_server : public CTest
{
public:
	void test(int argc, char ** argv)
	{
		WSAData data = { 0 };
		WSAStartup(MAKEWORD(2, 2), &data);

		SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (INVALID_SOCKET == sock)
		{
			cout << "socket failed" << endl;
		}

		struct sockaddr_in server_addr = { 0 };
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(8888);
		server_addr.sin_addr.S_un.S_addr = inet_addr("192.168.9.202");
		bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
		cout << "server start.." << endl;
		for (;;)
		{
			char buf[100] = { 0 };
			struct sockaddr_in client_addr = { 0 };
			int len = sizeof(client_addr);
			recvfrom(sock, buf, 100, 0, (struct sockaddr*)&client_addr, &len);
			
			sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&client_addr, len);
		}
	}
};


#endif