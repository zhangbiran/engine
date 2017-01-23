#include <WinSock2.h>
#include <windows.h>
#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

/*
int getsockopt(
_In_    SOCKET s,
_In_    int    level,		
_In_    int    optname,
_Out_   char   *optval,
_Inout_ int    *optlen
);


level:	
SOL_SOCKET


optname:
SO_ACCEPTCONN
*/

const bool USETCP = true;

int main(void)
{
	WSAData data = { 0 };
	WSAStartup(MAKEWORD(2, 2), &data);
	int message_type = USETCP ? SOCK_STREAM : SOCK_DGRAM;
	int proto_type = USETCP ? IPPROTO_TCP : IPPROTO_UDP;

	SOCKET sock = socket(AF_INET, message_type, proto_type);
	if (SOCKET_ERROR == sock)
	{
		cout << "socket failed" << endl;
		return -1;
	}

	

	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.S_un.S_addr = inet_addr("192.168.9.202");
	if (SOCKET_ERROR == bind(sock, (sockaddr*)&addr, sizeof(sockaddr_in)))
	{
		cout << "bind failed" << endl;
		cout << WSAGetLastError() << endl;
		return -1;
	}

	if (USETCP)
	{
		if (SOCKET_ERROR == listen(sock, 10))
		{
			cout << "listen failed" << endl;
			return -1;
		}

		

		//判断是否处于监听状态
		BOOL ral;
		int len = sizeof(BOOL);
		if (SOCKET_ERROR == getsockopt(sock, SOL_SOCKET, SO_ACCEPTCONN, (char*)&ral, &len))
		{
			cout << "getsockopt failed" << endl;
			cout << WSAGetLastError() << endl;

			return -1;
		}
		cout << "SO_ACCEPTCONN: " << ral << endl;

		sockaddr_in client_addr = { 0 };
		int client_addr_size = sizeof(client_addr);
		cout << "accepting ..." << endl;
		SOCKET client_sock = accept(sock, (sockaddr*)&client_addr, &client_addr_size);
		if (SOCKET_ERROR == client_sock)
		{
			cout << "accpet failed: " << WSAGetLastError() << endl;
			return -1;
		}
		cout << inet_ntoa(client_addr.sin_addr) << " connected" << endl;

		//获取是否linger
		linger lin;
		int linLen = sizeof(linger);
		if (SOCKET_ERROR == getsockopt(client_sock, SOL_SOCKET, SO_LINGER, (char*)&lin, &linLen))
		{
			cout << "getsockopt failed in SO_LINGER: " << WSAGetLastError() << endl;
			return -1;
		}
		cout << "lin: " << lin.l_onoff << endl;
		cout << "lin: " << lin.l_linger << endl;

		Sleep(30000);
		closesocket(client_sock);
		cout << "closed socket" << endl;
		while (1)
		{
			Sleep(100);
		}
		
	}
	else
	{
		BOOL setBroadCast = TRUE;

		if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&setBroadCast, sizeof(BOOL)))
		{
			cout << "setsockopt failed: " << WSAGetLastError() << endl;
			return -1;
		}

		//
		BOOL ral2;
		int len2 = sizeof(BOOL);
		if (SOCKET_ERROR == getsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&ral2, &len2))
		{
			cout << "getsockopt failed" << endl;
			cout << WSAGetLastError() << endl;

			return -1;
		}
		cout << "SO_BROADCAST: " << ral2 << endl;
	}

	cout << "main end." << endl;
	return 0;
}