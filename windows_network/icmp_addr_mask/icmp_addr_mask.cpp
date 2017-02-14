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

	UINT addr_mask;//可选项
}IcmpHeader;

enum ICMP_CODE
{
	ICMP_ECHOREPLY = 0,		//ECHO回复
	ICMP_DESTUNREACH = 3,	//主机不可到达
	ICMP_SRCQUENCH = 4,
	ICMP_REDIRECT = 5,
	ICMP_ECHO = 8,			//ECHO请求
	ICMP_TIMEOUT = 11,		//ttl = 0弃包消息，由路由器发
	ICMP_PARAMETER = 12,
	ICMP_ADDR_MASK_REQUEST = 17,	//icmp 地址掩码请求
	ICMP_ADDR_MASK_REPLY = 18		//icmp 地址掩码应答
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
	char *network_ip = "255.255.255.255";
	WSAData data = { 0 };
	WSAStartup(MAKEWORD(2, 2), &data);

	SOCKET sockraw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (INVALID_SOCKET == sockraw)
	{
		cout << "socket failed: " << WSAGetLastError() << endl;
		return -1;
	}

	char *icmp_data = (char *)malloc(1024);
	memset(icmp_data, 0, 1024);
	IcmpHeader * icmp_hdr = (IcmpHeader *)icmp_data;
	icmp_hdr->bType = ICMP_ADDR_MASK_REQUEST;
	icmp_hdr->bCode = 0;
	icmp_hdr->nsCheckSum = 0;
	icmp_hdr->nsSeq = 1;
	icmp_hdr->usId = ::GetCurrentProcessId();
	icmp_hdr->addr_mask = 0;
	icmp_hdr->nsCheckSum = checksum((USHORT*)icmp_data,
		12);

	int datasize = DEF_PACKET_SIZE;
	datasize += sizeof(IcmpHeader);
	memset(icmp_data + sizeof(IcmpHeader), 'E', DEF_PACKET_SIZE);
	
	char *recvBuf = (char*)malloc(MAX_PACKET);
	memset(recvBuf, 0, MAX_PACKET);

	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(network_ip);

	if (SOCKET_ERROR == sendto(sockraw, icmp_data, datasize, 0, (sockaddr*)&addr, sizeof(addr)))
	{
		cout << "sendto failed: " << WSAGetLastError() << endl;
		return -1;
	}

	while (1)
	{
		sockaddr_in from_addr = { 0 };
		int len = sizeof(from_addr);
		int ret = recvfrom(sockraw, recvBuf, MAX_PACKET, 0, (sockaddr*)&from_addr, &len);
		if (SOCKET_ERROR == ret)
		{
			cout << "recvfrom failed: " << WSAGetLastError() << endl;
			return -1;
		}

		IpHeader * ip_hdr = (IpHeader*)recvBuf;
		int iphdrlen = ip_hdr->h_len * 4;
		cout << "ip_hdr->h_len = " << iphdrlen << endl;
		if (ret < (iphdrlen + ICMP_MIN))
		{
			cout << "packet length error !" << endl;
			return -1;
		}

		IcmpHeader *icmp_hdr = (IcmpHeader*)(recvBuf + iphdrlen);

		cout << inet_ntoa(from_addr.sin_addr) << endl;
		sockaddr_in temp = { 0 };
		memcpy(&temp.sin_addr, &icmp_hdr->addr_mask, sizeof(int));
		cout << inet_ntoa(temp.sin_addr) << endl;
	
	}

	return 0;
}