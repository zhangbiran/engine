#include <WinSock2.h>
#include <windows.h>
#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

bool isSend = false;	//发送者、接收者

int main(int argc, char ** argv)
{
	WSAData data = { 0 };
	WSAStartup(MAKEWORD(2, 2), &data);

	cout << "argc: " << argc << endl;
	if (argc > 1)
	{
		isSend = true;
	}

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SOCKET_ERROR == sock)
	{
		cout << "socket failed" << WSAGetLastError() << endl;
		return -1;
	}

	//重用地址端口
	BOOL reuseAddr = TRUE;
	if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseAddr, sizeof(BOOL)))
	{
		cout << "setsocketopt failed: " << WSAGetLastError() << endl;
		return -1;
	}

	//绑定地址和端口
	sockaddr_in hostAddr = { 0 };
	hostAddr.sin_family = AF_INET;
	hostAddr.sin_port = htons(8888);
	hostAddr.sin_addr.S_un.S_addr = inet_addr("192.168.9.202");
	if (SOCKET_ERROR == bind(sock, (sockaddr*)&hostAddr, sizeof(hostAddr)))
	{
		cout << "bind failed: " << WSAGetLastError() << endl;
		return -1;
	}
	//获取最大发送消息长度
	int maxMsgSize;
	int len = sizeof(int);
	if (SOCKET_ERROR == getsockopt(sock, SOL_SOCKET, SO_MAX_MSG_SIZE, (char*)&maxMsgSize, &len))
	{
		cout << "getsockopt failed" << endl;
		return -1;
	}
	cout << "maxMsgSize: " << maxMsgSize << endl;

	//设置广播权限
	BOOL canBroadCasst = TRUE;
	if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&canBroadCasst, sizeof(BOOL)))
	{
		cout << "setsockopt failed: " << WSAGetLastError() << endl;
		return -1;
	}

	//发送广播
	if (isSend)
	{
		sockaddr_in addr = { 0 };
		addr.sin_family = AF_INET;
		addr.sin_port = htons(8888);
		addr.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");
		char buf[100] = { 0 };
		while (cin.getline(buf, 100))
		{
			if (SOCKET_ERROR == sendto(sock, buf, cin.gcount(), 0, (sockaddr*)&addr, sizeof(addr)))
			{
				cout << "sendto failed: " << WSAGetLastError() <<  endl;
				return -1;
			}

			//广播后发送者也能接收到
			char recvBuf[100] = { 0 };
			sockaddr_in peerAddr = { 0 };
			int addLen = sizeof(sockaddr_in);
			int recvSize = recvfrom(sock, recvBuf, 100, 0, (sockaddr*)&peerAddr, &addLen);
			if (SOCKET_ERROR == recvSize)
			{
				cout << "recvfrom failed" << WSAGetLastError() << endl;
				return -1;
			}
			else if (0 == recvSize)
			{
				cout << "graceful close" << endl;
				break;
			}
			cout << recvBuf << ":" << recvSize << endl;
		}
	}
	else	//接收广播
	{
		char buf[100] = { 0 };
		sockaddr_in peerAddr = { 0 };
		int addLen = sizeof(sockaddr_in);
		int recvSize = 0;
		while ((recvSize = recvfrom(sock, buf, 100, 0, (sockaddr*)&peerAddr, &addLen)) > 0)
		{
			buf[addLen] = 0;
			cout << buf << ":" << recvSize << endl;
		}
		if (SOCKET_ERROR == recvSize)
		{
			cout << WSAGetLastError() << endl;
		}
	}


	cout << "main end." << endl;
	closesocket(sock);
	WSACleanup();

}