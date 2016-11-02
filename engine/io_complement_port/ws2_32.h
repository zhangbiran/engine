#ifndef __WS2_32_H
#define __WS2_32_H

#include <WinSock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
using namespace std;

class CSocket
{
public:
	CSocket(int family, int type, int protocol,
		char *ip, int port,int listen_count)
	{
		if (!_isStartUp)
		{
			cout << "no start up" << endl;
		}
		
	}

private:
	static bool _isStartUp;
	SOCKET _sock;
};

bool CSocket::_isStartUp = false;


#endif