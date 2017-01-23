#include <WinSock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")
#include <iostream>
using namespace std;

int main(void)
{
	WSAData data = { 0 };
	WSAStartup(MAKEWORD(2, 2, ), &data);

	HOSTENT  * host = gethostbyname("www.baidu.com");
	
	cout << "主机名: " <<  host->h_name << endl;
	cout << "别名: ";
	char **p = host->h_aliases;
	while (*p != NULL)
	{
		cout << *p << endl;
		p++;
	}
	cout << "地址类型: ";
	if (host->h_addrtype == AF_INET)
	{
		cout << "AF_INET" << endl;
	}
	else if (host->h_addrtype == AF_INET6)
	{
		cout << "AF_INET6" << endl;
	}

	cout << "ip :" << endl;
	char **ip = host->h_addr_list;
	while (*ip != NULL)
	{
		struct in_addr addr = { 0 };
		memcpy(&addr, *ip, host->h_length);
		cout << inet_ntoa(addr) << endl;
		ip++;
	}
	WSACleanup();
	return 0;
}