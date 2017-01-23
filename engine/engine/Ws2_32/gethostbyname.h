#ifndef __GET_HOST_BY_NAME_H
#define  __GET_HOST_BY_NAME_H
#include <test.h>

#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
using namespace std;


/*
相关文件
C:\Windows\System32\drivers\etc\hosts


*/

/*
struct hostent* FAR gethostbyname(
_In_ const char *name
);

/*
struct  hostent {
	char    FAR * h_name;				//official name of host
	char    FAR * FAR * h_aliases;		//alias list 
	short   h_addrtype;					//host address type 
	short   h_length;					//length of address 
	char    FAR * FAR * h_addr_list;	//list of addresses 
#define h_addr  h_addr_list[0]			//address, for backward compat
};

*/

class gethostbynameTest : public CTest
{
public:
	void test(int argc, char** argv)
	{
		WSAData data = { 0 };
		WSAStartup(MAKEWORD(2, 2), &data);

		char *name[] = { "www.sina.com", "www.baidu.com", "www.contoso.com", "www.sogou.com", 
			"localhost", "127.0.0.1", "mssplus.mcafee.com", "my.com"};
		int size = sizeof(name) / sizeof(char*);
		cout << size << endl;
		for (int i = 0; i < size; i++)
		{
			struct hostent * remoteHost = gethostbyname(name[i]); // www.contoso.com
			if (NULL == remoteHost)
			{
				WSACleanup();
				return;
			}
			cout << remoteHost->h_name << endl;
			char **pp = remoteHost->h_aliases;
			for (; *pp != NULL; pp++)
			{
				cout << *pp << endl;
			}
			switch (remoteHost->h_addrtype) {
			case AF_INET:
				printf("AF_INET\n");
				break;
			case AF_NETBIOS:
				printf("AF_NETBIOS\n");
				break;
			default:
				printf(" %d\n", remoteHost->h_addrtype);
				break;
			}

			cout << remoteHost->h_length << endl;
			for (char **ip = remoteHost->h_addr_list; *ip != NULL; ip++)
			{
				if (remoteHost->h_addrtype == AF_INET)
				{
					struct in_addr addr;
					addr.S_un.S_addr = *((unsigned long *)(*ip));
					cout << inet_ntoa(addr) << endl;
				}
				else if (remoteHost->h_addrtype == AF_INET6)
				{
					cout << "ip is AF_INET6" << endl;
				}
			}

			cout << endl;
			WSACleanup();
		}

		
	}
};



#endif