
#include <WinSock2.h>
#include <windows.h>
#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")


int main(void)
{
	WSAData data = { 0 };
	WSAStartup(MAKEWORD(2, 2), &data);

	SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (SOCKET_ERROR == sock)
	{
		cout << "socket error" << endl;
		return -1;
	}




	return 0;
}