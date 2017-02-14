#include <iostream>
using namespace std;

#include <WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

//IP头
typedef struct _ip_hdr
{
	unsigned char h_len : 4;
	unsigned char version : 4;
	unsigned char tos;
	unsigned short packet_len;
	unsigned short flag;
	unsigned short flag_pianyi;
	unsigned char ttl;
	unsigned char proto;
	unsigned short checksum;
	unsigned int srcIp;
	unsigned int destIp;
}IpHeader;

//IP 选项
typedef struct _ip_opt
{
	unsigned char type;
	unsigned char length;
	unsigned char offset;
	unsigned long addr[9];
}IpOption;

//ICMP头
typedef struct _icmp_hdr
{
	unsigned char type;
	unsigned char code;
	unsigned short checksum;
	unsigned short id;
	unsigned short seq;

	unsigned int timestamp;
}IcmpHeader;

#define DEF_PACKET_SIZE 32
#define ICMP_MIN 8
#define MAX_SIZE 1024

#define ICMP_ECHO_REPLY 0
#define ICMP_ECHO 8

USHORT checksum(USHORT *buffer, int size)
{
	unsigned long cksum = 0;

	while (size > 1)
	{
		cksum += *buffer++;
		size -= sizeof(USHORT);
	}
	if (size)
	{
		cksum += *(UCHAR*)buffer;
	}
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (USHORT)(~cksum);
}

int main(int argc, char ** argv)
{
	WSAData data = { 0 };
	WSAStartup(MAKEWORD(2, 2), &data);

	if (argc < 2)
	{
		cout << "argc < 2" << endl;
		return -1;
	}

	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	if (INADDR_NONE == addr.sin_addr.S_un.S_addr)
	{
		HOSTENT *host = gethostbyname(argv[1]);
		if (NULL == host)
		{
			cout << "can't resolve addr: " << argv[1] << endl;
			return -1;
		}

		memcpy(&addr.sin_addr.S_un.S_addr, host->h_addr_list[0], host->h_length);
	}

	cout << "ping " << inet_ntoa(addr.sin_addr) << endl;

	SOCKET sockraw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (SOCKET_ERROR == sockraw)
	{
		cout << "socket failed: " << WSAGetLastError() << endl;
		return -1;
	}

	IpOption ip_opt = { 0 };
	ip_opt.type = 0x7;
	ip_opt.length = 39;
	ip_opt.offset = 4;
	if (SOCKET_ERROR == setsockopt(sockraw, IPPROTO_IP, IP_OPTIONS, (char*)&ip_opt, sizeof(ip_opt)))
	{
		cout << "IP_OPTIONS failed: " << WSAGetLastError() << endl;
		return -1;
	}

	int timetout = 3000;
	if (setsockopt(sockraw, SOL_SOCKET, SO_SNDTIMEO, (char*)&timetout, sizeof(timetout)))
	{
		cout << "SO_SNDTIMEO failed: " << WSAGetLastError() << endl;
		return -1;
	}

	timetout = 3000;
	if (setsockopt(sockraw, SOL_SOCKET, SO_RCVTIMEO, (char*)&timetout, sizeof(timetout)))
	{
		cout << "SO_RCVTIMEO failed: " << WSAGetLastError() << endl;
		return -1;
	}

	char *icmp_data = (char*)malloc(MAX_SIZE);
	char *rcvBuf = (char*)malloc(MAX_SIZE);
	memset(icmp_data, 0, MAX_SIZE);
	memset(rcvBuf, 0, MAX_SIZE);

	((IcmpHeader*)icmp_data)->type = ICMP_ECHO;
	((IcmpHeader*)icmp_data)->id = GetCurrentProcessId();
	char *part = icmp_data + sizeof(IcmpHeader);
	memset(part, 'E', DEF_PACKET_SIZE);


	int datasize = DEF_PACKET_SIZE + sizeof(IcmpHeader);
	bool print_router = false;
	for (int i = 0; i < 10; ++i)
	{
		((IcmpHeader*)icmp_data)->code = 0;
		((IcmpHeader*)icmp_data)->checksum = 0;
		((IcmpHeader*)icmp_data)->checksum = checksum((USHORT*)icmp_data, datasize);
		((IcmpHeader*)icmp_data)->seq = i;
		((IcmpHeader*)icmp_data)->timestamp = ::GetTickCount();
		
		if (SOCKET_ERROR == sendto(sockraw, icmp_data, datasize, 0, (sockaddr*)&addr, sizeof(addr)))
		{
			cout << "sendto failed: " << WSAGetLastError() << endl;
			return -1;
		}

		sockaddr_in from_addr = { 0 };
		int len = sizeof(from_addr);

		int ret = recvfrom(sockraw, rcvBuf, MAX_SIZE, 0, (sockaddr*)&from_addr, &len);
		if (SOCKET_ERROR == ret)
		{
			if (WSAETIMEDOUT == WSAGetLastError())
			{
				cout << "time out" << endl;
				continue;
			}
		}

		if (!print_router)
		{
			IpHeader * iphdr = (IpHeader*)rcvBuf;
			int iphdr_len = iphdr->h_len * 4;
			IpOption *opt = (IpOption*)iphdr;
			cout << "opt->offset= " << opt->offset << endl;

			print_router = true;
		}

		Sleep(1000);
	}



	WSACleanup();
	return 0;
}