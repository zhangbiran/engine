#include <winsock.h>	//winsock1版本
#include <windows.h>
#pragma comment(lib, "Wsock32.lib")	//winsock1版本

#include <iostream>
using namespace std;


bool isSend = false;	//发送者、接收者
#define MULTICAST_ADDR "234.5.6.7"
#define LOCAL_ADDR "192.168.9.202"
#define PORT 8888


int main(int argc, char ** argv)
{
	WSAData data = { 0 };
	WSAStartup(MAKEWORD(1, 1), &data);	//注意这里是1,1, winsock1版本

	cout << "argc: " << argc << endl;
	if (argc > 1)
	{
		isSend = true;
	}

	if (isSend)
	{
		cout << "is sender" << endl;
	}
	else
	{
		cout << "is receiver" << endl;
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
	hostAddr.sin_port = htons(PORT);
	hostAddr.sin_addr.S_un.S_addr = inet_addr(LOCAL_ADDR);
	if (SOCKET_ERROR == bind(sock, (sockaddr*)&hostAddr, sizeof(hostAddr)))
	{
		cout << "bind failed: " << WSAGetLastError() << endl;
		return -1;
	}

	//加入多播组
	struct ip_mreq      mcast;
	mcast.imr_multiaddr.S_un.S_addr = inet_addr(MULTICAST_ADDR);
	mcast.imr_interface.S_un.S_addr = inet_addr(LOCAL_ADDR);

	if (SOCKET_ERROR == setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mcast, sizeof(mcast)))
	{
		cout << "setsockopt IP_ADD_MEMBERSHIP failed" << WSAGetLastError() << endl;
		return -1;
	}

	//设置TTL
	int ttl = 8;
	if (SOCKET_ERROR == setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl, sizeof(int)))
	{
		cout << "setsockopt IP_MULTICAST_TTL failed: " << WSAGetLastError() << endl;
		return -1;
	}

	if (isSend)
	{
		sockaddr_in remote = { 0 };
		remote.sin_family = AF_INET;
		remote.sin_port = htons(PORT);
		remote.sin_addr.S_un.S_addr = inet_addr(MULTICAST_ADDR);
		char buf[100] = { 0 };
		while (cin.getline(buf, 100))
		{
			if (SOCKET_ERROR == sendto(sock, buf, cin.gcount(), 0, (sockaddr*)&remote, sizeof(remote)))
			{
				cout << "sendto failed: " << WSAGetLastError() << endl;
				break;
			}
		}
	}
	else
	{
		char buf[100] = { 0 };
		while (1)
		{
			sockaddr_in remote = { 0 };
			int len = sizeof(remote);
			int ret = recvfrom(sock, buf, 100, 0, (sockaddr*)&remote, &len);
			if (SOCKET_ERROR == ret)
			{
				cout << "recvfrom failed: " << WSAGetLastError() << endl;
				break;
			}
			else if (0 == ret)
			{
				cout << "graceful closed: " << endl;
				break;
			}
			buf[ret] = 0;
			cout << buf << ": " << ret << endl;
		}
	}

	//退出多播组
	if (SOCKET_ERROR == setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mcast, sizeof(mcast)))
	{
		cout << "setsockopt IP_DROP_MEMBERSHIP failed: " << WSAGetLastError() << endl;
		closesocket(sock);
		return -1;
	}

	WSACleanup();

	return 0;
}