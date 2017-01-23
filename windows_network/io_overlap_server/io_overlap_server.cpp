#include <WinSock2.h>
#include <windows.h>
#include <tchar.h>
#include <iostream>
using namespace std;

#include <ws2tcpip.h>
#include <svcguid.h>
#include <wsipx.h>
#include <wsnwlink.h> 
#include <nspapi.h>

#pragma comment(lib, "ws2_32.lib")
struct MyInfo
{
	SOCKET s;
	WSABUF wsaBuf;
	WSAOVERLAPPED wsaOverlapped;
	char buf[100];
};

CRITICAL_SECTION critical_section = { 0 };
DWORD total_event = 0;
WSAEVENT event[WSA_MAXIMUM_WAIT_EVENTS] = { 0 };
MyInfo * myinfo[WSA_MAXIMUM_WAIT_EVENTS] = { 0 };


DWORD CALLBACK threadProc(void *arg)
{
	while (1)
	{
		DWORD index = WSAWaitForMultipleEvents(total_event, event, FALSE, WSA_INFINITE, FALSE);
		if (WSA_WAIT_FAILED == index)
		{
			cout << "WSAWaitForMultipleEvents Failed" << endl;
			return -1;
		}

		if (index - WSA_WAIT_EVENT_0 == 0)
		{
			WSAResetEvent(event[0]);
			continue;
		}

		MyInfo *info = myinfo[index - WSA_WAIT_EVENT_0];
		WSAResetEvent(event[index - WSA_WAIT_EVENT_0]);

		DWORD rcvSize = 0;
		DWORD flag = 0;
		if (WSAGetOverlappedResult(info->s, &info->wsaOverlapped, &rcvSize, FALSE, &flag) == FALSE)
		{
			cout << "WSAGetOverlappedResult Failed" << endl;
			return -1;
		}

		if (0 == rcvSize)
		{
			closesocket(info->s);
			delete info;
			WSACloseEvent(event[index - WSA_WAIT_EVENT_0]);
			return -1;
		}

		cout << info->buf << ":" << rcvSize << endl;
	}

	return 0;
}

int main(void)
{
	WSAData data = { 0 };
	WSAStartup(MAKEWORD(2, 2), &data);

	InitializeCriticalSection(&critical_section);

	SOCKET sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (SOCKET_ERROR == sock)
	{
		cout << "socket Failed" << endl;
		return -1;
	}

	sockaddr_in server_addr = { 0 };
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(7776);
	server_addr.sin_addr.S_un.S_addr = inet_addr("192.168.9.202");
	if (SOCKET_ERROR == bind(sock, (sockaddr*)&server_addr, sizeof(server_addr)))
	{
		cout << "bind Failed" << endl;
		return -1;
	}

	if (SOCKET_ERROR == listen(sock, 5))
	{
		cout << "listen Failed" << endl;
		return -1;
	}

	if ((event[0] = WSACreateEvent()) == WSA_INVALID_EVENT)
	{
		cout << "WSACreateEvent Failed" << endl;
		return -1;
	}
	total_event = 1;

	DWORD thread_id;
	HANDLE thread_handle = CreateThread(NULL, 0, threadProc, NULL, 0, &thread_id);
	if (NULL == thread_handle)
	{
		cout << "CreateThread Failed" << endl;
		return -1;
	}

	SOCKET accept_sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (SOCKET_ERROR == accept_sock)
	{
		cout << "WSASocket Failed" << endl;
		return -1;
	}
	
	cout << "accpet ..." << endl;
	while (1)
	{
		sockaddr_in client_addr = { 0 };
		int len = sizeof(client_addr);
		accept_sock = accept(sock, (sockaddr*)&client_addr, &len);
		if (INVALID_SOCKET == accept_sock)
		{
			cout << "accept Failed" << endl;
			break;
		}

		EnterCriticalSection(&critical_section); 

		event[total_event] = WSACreateEvent();
		if (WSA_INVALID_EVENT == event[total_event])
		{
			cout << "WSACreateEvent Failed" << endl;
			return -1;
		}
		
		myinfo[total_event] = new MyInfo();
		ZeroMemory(myinfo[total_event], sizeof(MyInfo));
		myinfo[total_event]->s = accept_sock;
		myinfo[total_event]->wsaBuf.buf = myinfo[total_event]->buf;
		myinfo[total_event]->wsaBuf.len = 100;
		myinfo[total_event]->wsaOverlapped.hEvent = event[total_event];

		++total_event;

		DWORD rcvSize = 0;
		DWORD flag = 0;
		if (SOCKET_ERROR == WSARecv(myinfo[total_event]->s, &(myinfo[total_event]->wsaBuf), 1, &rcvSize, &flag, &(myinfo[total_event]->wsaOverlapped), NULL))
		{
			cout << "WSARecv Failed: " << WSAGetLastError() << endl;
			return -1;
		}

		LeaveCriticalSection(&critical_section);
		WSASetEvent(event[0]);
	}                                                                                                                                                                          


	return 0;
}