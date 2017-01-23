#include <winsock2.h>	//winsock2版本
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")	//winsock2版本

#include <iostream>
using namespace std;


bool isSend = false;	//发送者、接收者
#define MULTICAST_ADDR "234.5.6.7"
#define LOCAL_ADDR "192.168.9.202"
#define PORT 8888


int main(int argc, char ** argv)
{
	if (argc > 1)
	{
		isSend = true;
		cout << "is sender" << endl;
	}
	else
	{
		cout << "is receiver" << endl;
	}

	WSAData data = { 0 };
	WSAStartup(MAKEWORD(2, 2), &data);

	SOCKET sock = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0,
		WSA_FLAG_MULTIPOINT_C_LEAF |
		WSA_FLAG_MULTIPOINT_D_LEAF |
		WSA_FLAG_OVERLAPPED
		);
	
	//SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	//不可以，必须用WSASocket
	if (SOCKET_ERROR == sock)
	{
		cout << "WSASocket failed: " << WSAGetLastError() << endl;
		return -1;
	}

	//重用地址端口
	BOOL reuseAddr = TRUE;
	if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseAddr, sizeof(BOOL)))
	{
		cout << "setsocketopt failed: " << WSAGetLastError() << endl;
		return -1;
	}

	//绑定
	sockaddr_in local_addr = { 0 };
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(PORT);
	local_addr.sin_addr.S_un.S_addr = inet_addr(LOCAL_ADDR);
	if (SOCKET_ERROR == bind(sock, (sockaddr*)&local_addr, sizeof(local_addr)))
	{
		cout << "bind failed: " << endl;
		return -1;
	}

	//设置TTL
	int ttl = 8;
	if (SOCKET_ERROR == setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl, sizeof(int)))
	{
		cout << "setsockopt IP_MULTICAST_TTL failed" << endl;
		return -1;
	}

	//加入多播组
	sockaddr_in remote = { 0 };
	remote.sin_family = AF_INET;
	remote.sin_port = htons(PORT);
	remote.sin_addr.S_un.S_addr = inet_addr(MULTICAST_ADDR);
	SOCKET multipointsocket = WSAJoinLeaf(sock, (sockaddr*)&remote, sizeof(remote), NULL, NULL, NULL, NULL, JL_BOTH);
	if (INVALID_SOCKET == multipointsocket)	//注意，这里不是SOCKET_ERROR
	{
		cout << "WSAJoinLeaf failed: " << WSAGetLastError() << endl;
		return -1;
	}

	if (isSend)
	{
		char buf[100] = { 0 };
		while (cin.getline(buf, 100))
		{
			if (SOCKET_ERROR == sendto(multipointsocket, buf, cin.gcount(), 0, (sockaddr*)&remote, sizeof(remote)))
			{
				cout << "sendto failed: " << endl;
				break;
			}

		}
	}
	else
	{
		char buf[100] = { 0 };
		while (1)
		{
			sockaddr_in from_addr = { 0 };
			int len = sizeof(from_addr);
			int ret = recvfrom(multipointsocket, buf, 100, 0, (sockaddr*)&from_addr, &len);
			if (SOCKET_ERROR == ret)
			{
				cout << "recvfrom failed: " << WSAGetLastError() << endl;
				break;
			}
			else if (0 == ret)
			{
				cout << "greceful closed: " << endl;
				break;
			}
			else
			{
				buf[ret] = 0;
				cout << buf << ": " << ret << endl;
			}
		}

	}

	closesocket(multipointsocket);
	closesocket(sock);
	
	WSACleanup();
	return -1;
}