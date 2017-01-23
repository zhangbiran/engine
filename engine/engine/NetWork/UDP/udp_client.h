#ifndef __UDP_CLIENT_H
#define __UDP_CLIENT_H
#include "test.h"
#include <windows.h>
#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

/*
udp��socketҲ�ǿ���ʹ��connect�����ģ����Լ�������߷���Ч�ʣ�����ֻ�ܺ�һ��server����ͨ��


*/

class UDP_client : public CTest
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
		cout << "client start.." << endl;

		bool useConnectUdp = false;
		{
			if (SOCKET_ERROR == connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)))
			{
				cout << "connect failed" << endl;
				return;
			}
			useConnectUdp = true;
		}

		char buf[100] = { 0 };
		while (cin.getline(buf, 100))
		{	
			char recvBuf[100] = { 0 };
			if (!useConnectUdp)
			{
				sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
				recvfrom(sock, recvBuf, 100, 0, NULL, NULL);
			}
			else
			{
				if (SOCKET_ERROR == send(sock, buf, strlen(buf), 0))
				{
					cout << "send failed errno: " << WSAGetLastError() << endl;
					return;
				}
			
				if (SOCKET_ERROR == recv(sock, recvBuf, sizeof(recvBuf), 0))
				{
					cout << "recv failed errno: " << WSAGetLastError() << endl;
					return;
				}
			}
		
			cout << recvBuf << endl;
		}
	}
};



#endif