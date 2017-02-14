#include <iostream>
using namespace std;
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")
//IP头
typedef struct __iphdr
{
	unsigned char h_len : 4;		//ip头长度
	unsigned char version : 4;	//
	unsigned char tos;		//服务类型
	unsigned short total_len;//包长度
	unsigned short ident;	//唯一标识
	unsigned short flag_and_pianyi;	//标志和偏移
	unsigned char ttl;		//生存时间
	unsigned char proto;	//协议(tcp/udp etc)
	unsigned short checksum;//校验和
	unsigned int sourceIp;	//源ip地址
	unsigned int destIp;	//目地IP地址
}IpHeader;


//ICMP头
typedef struct _icmphdr
{
	BYTE bType;	//类型
	BYTE bCode;	//编码
	USHORT nsCheckSum;	//校验和
	USHORT usId;	//标识符
	USHORT nsSeq;	//序列号
	UINT niTimeStamp;//可选项
}IcmpHeader;

enum ICMP_CODE
{
	ICMP_ECHOREPLY = 0,		//ECHO回复
	ICMP_DESTUNREACH = 3,	//主机不可到达
	ICMP_SRCQUENCH = 4,	
	ICMP_REDIRECT = 5,
	ICMP_ECHO = 8,			//ECHO请求
	ICMP_TIMEOUT = 11,		//ttl = 0弃包消息，由路由器发
	ICMP_PARAMETER = 12
};

#define MAX_HOPS 30
#define ICMP_MIN 8
#define DEF_PACKET_SIZE 32
#define MAX_PACKET 1024

USHORT checksum(USHORT *buffer, int size)
{
	unsigned long cksum = 0;

	while (size > 1)
	{
		cksum += *buffer++;
		size -= sizeof(USHORT);
	}
	if (size)
		cksum += *(UCHAR*)buffer;
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);

	return (USHORT)(~cksum);
}


int main(int argc, char ** argv)
{
	if (argc < 2)
	{
		cout << "erorr: argc < 2" << endl;
		return -1;
	}

	WSAData data = { 0 };
	WSAStartup(MAKEWORD(2, 2), &data);
	
	sockaddr_in addr = { 0 };
	addr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	if (INADDR_NONE == addr.sin_addr.S_un.S_addr)
	{
		HOSTENT *hostent = gethostbyname(argv[1]);
		if (NULL == hostent)
		{
			printf("Unable to resolve %s\n", argv[1]);
			return -1;
		}

		memcpy(&addr.sin_addr, hostent->h_addr_list[0], hostent->h_length);
		cout << "跟踪" << hostent->h_name << "(" << inet_ntoa(addr.sin_addr) << ")" <<"的路由路径:" << endl;
	}
	addr.sin_family = AF_INET;

	SOCKET sockraw = //WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP,
		//NULL, 0, WSA_FLAG_OVERLAPPED);
	socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (INVALID_SOCKET == sockraw)
	{
		cout << "socket failed: " << WSAGetLastError() << endl;
		return -1;
	}

	BOOL opt = TRUE;
	if (SOCKET_ERROR == setsockopt(sockraw, SOL_SOCKET, SO_DONTROUTE, (char*)&opt, sizeof(opt)))
	{
		cout << "SO_DONTROUTE failed: " << WSAGetLastError() << endl;
		return -1;
	}

	int timeout = 2000;
	if (SOCKET_ERROR == setsockopt(sockraw, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout)))
	{
		cout << "SO_SNDTIMEO failed: " << WSAGetLastError() << endl;
		return -1;
	}

	timeout = 2000;
	if (SOCKET_ERROR == setsockopt(sockraw, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)))
	{
		cout << "SO_RCVTIMEO failed: " << WSAGetLastError() << endl;
		return -1;
	}

	char *icmp_data = (char *)malloc(1024);
	memset(icmp_data, 0, 1024);
	IcmpHeader * icmp_hdr = (IcmpHeader *)icmp_data;
	icmp_hdr->bType = ICMP_ECHO;
	icmp_hdr->bCode = 0;
	icmp_hdr->nsCheckSum = 0;
	icmp_hdr->nsSeq = 0;
	icmp_hdr->usId = ::GetCurrentProcessId();
	icmp_hdr->niTimeStamp = 0;

	int datasize = DEF_PACKET_SIZE;
	datasize += sizeof(IcmpHeader);
	memset(icmp_data + sizeof(IcmpHeader), 'E', DEF_PACKET_SIZE);

	char *recvBuf = (char*)malloc(MAX_PACKET);
	memset(recvBuf, 0, MAX_PACKET);

	int seq_no = 0;
	sockaddr_in from_addr = { 0 };
	int len = sizeof(from_addr);
	for (int ttl = 1; ttl < MAX_HOPS; ++ttl)
	{

		//
		int ttl2 = ttl;
		if (SOCKET_ERROR == setsockopt(sockraw, IPPROTO_IP, IP_TTL, (char*)&ttl2, sizeof(ttl2)))
		{
			cout << "IP_TTL failed: " << WSAGetLastError() << endl;
			return -1;
		}

		((IcmpHeader*)icmp_data)->nsCheckSum= 0;
		((IcmpHeader*)icmp_data)->niTimeStamp = GetTickCount();
		//cout << "tick: " << ((IcmpHeader*)icmp_data)->niTimeStamp << endl;
		int tempTimeStamp = ((IcmpHeader*)icmp_data)->niTimeStamp;

		((IcmpHeader*)icmp_data)->nsSeq = seq_no++;
		((IcmpHeader*)icmp_data)->nsCheckSum = checksum((USHORT*)icmp_data,
			datasize);

		if (SOCKET_ERROR == sendto(sockraw, icmp_data, datasize, 0, (sockaddr*)&addr, sizeof(addr)))
		{
			cout << "sendto failed: " << WSAGetLastError() << endl;
			return -1;
		}

		memset(recvBuf, 0, MAX_PACKET);
		int ret = recvfrom(sockraw, recvBuf, MAX_PACKET, 0, (sockaddr*)&from_addr, &len);
		if (SOCKET_ERROR == ret)
		{
			if (WSAETIMEDOUT == WSAGetLastError())
			{
				cout << "ttl= " << ttl << " Receive Request time out." << endl;
				continue;
			}
			else
			{
				cout << "recvfrom failed: " << WSAGetLastError() << endl;
				return -1;
			}
		}
		//cout << "ret: " << ret << endl;
		IpHeader * ip_hdr = (IpHeader*)recvBuf;
		int iphdrlen = ip_hdr->h_len * 4;
		//cout << "ip_hdr->h_len = " << iphdrlen << endl;
		if (ret < (iphdrlen + ICMP_MIN))
		{
			cout << "packet length error !" << endl;
			return -1;
		}

		IcmpHeader * icmp = (IcmpHeader*)(recvBuf + iphdrlen);
		if (icmp->bType == ICMP_ECHOREPLY)
		{
			HOSTENT *from_host = gethostbyaddr((char*)&(from_addr.sin_addr), sizeof(from_addr.sin_addr), AF_INET);
			if (from_host != NULL)
			{
				cout << "reached " << inet_ntoa(from_addr.sin_addr) << "(" << from_host->h_name << ")" << endl;
			}
			else
			{
				cout << "ttl= " << ttl << ": " << inet_ntoa(addr.sin_addr) << endl;
			}
			cout << "end." << endl;
			break;
		}
		else if (icmp->bType == ICMP_TIMEOUT)
		{
			cout << "ttl= " << ttl << ": " << inet_ntoa(from_addr.sin_addr) << endl;
		}


		Sleep(1000);
	}


	return 0;
}