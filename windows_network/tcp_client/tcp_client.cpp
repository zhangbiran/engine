
#include <WinSock2.h>
#include <windows.h>
#include <iostream>
using namespace std;

#include <MSWSock.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")

int main(void)
{
	WSAData data = { 0 };
	WSAStartup(MAKEWORD(2, 2), &data);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sock)
	{
		cout << "socket failed" << WSAGetLastError() << endl;
		return -1;
	}
	sockaddr_in hostAddr = { 0 };
	hostAddr.sin_family = AF_INET;
	hostAddr.sin_port = htons(9999);
	hostAddr.sin_addr.S_un.S_addr = inet_addr("192.168.9.202");
	if (SOCKET_ERROR == bind(sock, (sockaddr*)&hostAddr, sizeof(hostAddr)))
	{
		cout << "bind failed: " << WSAGetLastError() << endl;
		return -1;
	}


	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.S_un.S_addr = inet_addr("192.168.9.202");

	if (SOCKET_ERROR == connect(sock, (sockaddr*)&addr, sizeof(addr)))
	{
		cout << "connected failed: " << WSAGetLastError() << endl;
		return -1;
	}

	while (1)
	{
		int connected_time = 0;
		int len = sizeof(int);
		//获取已连接的时间(秒)
		if (SOCKET_ERROR == getsockopt(sock, SOL_SOCKET, SO_CONNECT_TIME, (char*)&connected_time, &len))
		{
			cout << "getsockopt failed: " << WSAGetLastError() << endl;
			break;
		}

		if (connected_time == 20)
		{
			closesocket(sock);
		}

		if (0xffffffff == connected_time)
		{
			cout << "connection closed" << endl;
			break;
		}

		cout << connected_time << endl;
		Sleep(1000);
	}

	closesocket(sock);
	WSACleanup();
	return -1;
}