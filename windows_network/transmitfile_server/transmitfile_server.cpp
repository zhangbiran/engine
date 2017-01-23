#include <iostream>
using namespace std;
#include <WinSock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

int main(void)
{
	WSAData data = { 0 };
	WSAStartup(MAKEWORD(2, 2), &data);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sock)
	{
		cout << "socket failed" << endl;
		return -1;
	}

	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (SOCKET_ERROR == bind(sock, (sockaddr*)&addr, sizeof(addr)));
	{
		cout << "bind failed" << endl;
		return -1;
	}

	if (SOCKET_ERROR == listen(sock, 10))
	{
		cout << "listen failed" << endl;
		return -1;
	}

	while (1)
	{
		sockaddr_in client_addr = { 0 };
		int len = sizeof(client_addr);
		SOCKET client_sock = accept(sock, (sockaddr*)&client_addr, &len);
		if (SOCKET_ERROR == client_sock)
		{
			cout << "accept failed" << endl;
			break;
		}
	}
	INFINITE
		SOL_SOCKET
		SO_BROADCAST
	cout << "main end." << endl;

	return 0;
}